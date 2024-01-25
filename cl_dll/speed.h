#pragma once

class CHudSpeed : public CHudBase
{
public:
	bool Init() override;
	bool VidInit() override;
	bool Draw(float fTime) override;
	void Reset() override;
	bool MsgFunc_Speed(const char* pszName, int iSize, void* pbuf);
	int m_iSpeed;
	float m_fFade;
};
