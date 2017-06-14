#pragma once

class CAutoLive
{
	stringt m_pageURL;
	stringt m_bbsURL;
	stringt m_chargeURL;
	stringt m_newVersionURL;
	stringt m_kefuURL;
	stringt m_offPageURL;
	
	bool    m_bHasNewVersion;
	__int64 GetThisVersion();
	bool getThisVersionString(stringt & verString);
	__int64 GetOnlineVersion();
	__int64 GetStringVersion(LPCTSTR pVersion);
	bool GetVersionString(__int64 ver,stringt& verString);
public:
	CAutoLive(void);
	virtual ~CAutoLive(void);
	void update();
	bool GetCurrentPageURL(stringt & URL);
	bool GetBBSURL(stringt& bbsURL);
	bool GetChargeURL(stringt& chargeURL);
	bool GetKefuURL(stringt& keful);
	bool GetOffPageURL(stringt& OffPageURL);
	bool HasNewVersion();
	bool getVersionExe(stringt& strExe);
};
