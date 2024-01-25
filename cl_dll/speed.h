#pragma once

class CHudSpeed : public CHudBase
{
public:
	bool Init() override;
	bool VidInit() override;
	bool Draw(float fTime) override;
	void Reset() override;
	bool MsgFunc_Velocity(const char* pszName, int iSize, void* pbuf);
	int m_iSpeed;
	int m_iGroundSpeed;
	float m_fGroundColorFade;
	bool m_bGrounded;
	bool m_bGroundSpeedGain;
	float m_fFade;
};
