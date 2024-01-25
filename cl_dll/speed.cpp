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

DECLARE_MESSAGE(m_Speed, Velocity)

constexpr float groundFadeTime = 20.0f;

bool CHudSpeed::Init()
{
	HOOK_MESSAGE(Velocity);

	Reset();
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
		m_fFade -= gHUD.m_flTimeDelta * 20;
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

	if (m_fGroundColorFade != 0.0f)
	{
		m_fGroundColorFade -= gHUD.m_flTimeDelta * 20;
		m_fGroundColorFade = std::fmaxf(m_fGroundColorFade, 0.0f);

		const auto t = m_fGroundColorFade / groundFadeTime;

		int gr, gg, gb;
		UnpackRGB(gr, gg, gb, m_bGroundSpeedGain ? RGB_GREENISH : RGB_REDISH);

		// Interpolate indicator color over time
		r = t * gr + (1.0f - t) * r;
		g = t * gg + (1.0f - t) * g;
		b = t * gb + (1.0f - t) * b;
	}

	ScaleColors(r, g, b, a);

	SpeedWidth = gHUD.GetSpriteRect(gHUD.m_HUD_number_0).right - gHUD.GetSpriteRect(gHUD.m_HUD_number_0).left;

	y = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight / 2;

	x = (ScreenWidth + SpeedWidth) / 2;

	// Reserve space for 3 digits by default, but allow it to expand
	x += gHUD.GetHudNumberWidth(m_iSpeed, 3, DHN_DRAWZERO);

	gHUD.DrawHudNumberReverse(x, y, m_iSpeed, DHN_DRAWZERO, r, g, b);
	gHUD.DrawHudNumberReverse(x, y - gHUD.m_iFontHeight - gHUD.m_iFontHeight / 2, m_iGroundSpeed, DHN_DRAWZERO, r, g, b);

	x += SpeedWidth / 2;

	return true;
}

void CHudSpeed::Reset()
{
	m_iSpeed = 0;
	m_iGroundSpeed = 0;
	m_fGroundColorFade = 0.0f;
	m_bGrounded = true;
	m_bGroundSpeedGain = false;
}

bool CHudSpeed::MsgFunc_Velocity(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);

	const auto vx = READ_FLOAT();
	const auto vy = READ_FLOAT();
	const auto vz = READ_FLOAT();

	const auto isGrounded = std::fabsf(vz) < 0.1;
	if (m_bGrounded != isGrounded && m_iGroundSpeed != m_iSpeed)
	{
		m_bGrounded = isGrounded;
		m_bGroundSpeedGain = m_iSpeed > m_iGroundSpeed;
		m_iGroundSpeed = m_iSpeed;
		m_fFade = FADE_TIME;
		m_fGroundColorFade = groundFadeTime;
	}

	const auto xySpeed = static_cast<int>(Length(Vector(vx, vy, 0.0f)));
	if (m_iSpeed != xySpeed)
	{
		m_iSpeed = xySpeed;
		m_fFade = FADE_TIME;
	}

	return true;
}
