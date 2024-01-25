#include "stdio.h"
#include "stdlib.h"

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>

#include "stdio.h"
#include "stdlib.h"

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>

DECLARE_MESSAGE(m_Speed, Speed)

bool CHudSpeed::Init()
{
	HOOK_MESSAGE(Speed);

	m_iSpeed = 0;
	m_fFade = 0.0f;
	m_iFlags |= HUD_ACTIVE;

	gHUD.AddHudElem(this);
	return true;
}

bool CHudSpeed::VidInit()
{
	return true;
}

bool CHudSpeed::Draw(float flTime)
{
	int r, g, b;
	int a = 0, x, y;
	int SpeedWidth;

	if (0 != gEngfuncs.IsSpectateOnly())
		return true;

	// Has speed changed? Flash the speed #
	if (0 != m_fFade)
	{
		m_fFade -= (gHUD.m_flTimeDelta * 20);
		if (m_fFade <= 0)
		{
			a = MIN_ALPHA;
			m_fFade = 0;
		}

		// Fade the speed number back to dim
		a = MIN_ALPHA + (m_fFade / FADE_TIME) * 128;
	}
	else
	{
		a = MIN_ALPHA;
	}

	UnpackRGB(r, g, b, RGB_YELLOWISH);
	ScaleColors(r, g, b, a);

	SpeedWidth = gHUD.GetSpriteRect(gHUD.m_HUD_number_0).right - gHUD.GetSpriteRect(gHUD.m_HUD_number_0).left;

	y = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight / 2;

	x = (ScreenWidth + SpeedWidth) / 2;

	// Reserve space for 3 digits by default, but allow it to expand
	x += gHUD.GetHudNumberWidth(m_iSpeed, 3, DHN_DRAWZERO);

	gHUD.DrawHudNumberReverse(x, y, m_iSpeed, DHN_DRAWZERO, r, g, b);

	x += SpeedWidth / 2;

	return true;
}

void CHudSpeed::Reset()
{
}

bool CHudSpeed::MsgFunc_Speed(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);

	const auto speed = static_cast<int>(READ_FLOAT());
	if (speed != m_iSpeed)
	{
		m_iSpeed = speed;
		m_fFade = FADE_TIME;
	}

	return true;
}
