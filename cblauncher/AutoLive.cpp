#include "StdAfx.h"
#include "AutoLive.h"
#include "HttpWinInet.h"
#include "MemIni.h"
#include "resource.h"


#define EXE_URL_KEY		_T("exe")
#define VER_URL_KEY		_T("ver")
#define PAGE_URL_KEY	_T("page")
#define OFFPAGE_URL_KEY _T("offpage")
#define BBS_URL_KEY		_T("bbs")
#define KEFU_URL_KEY    _T("kefu")
#define CHARGE_URL_KEY	_T("charge")


#define VERSION_SECTION _T("VERSION")

CAutoLive::CAutoLive(void)
{
	m_bHasNewVersion  = false;

}

CAutoLive::~CAutoLive(void)
{
}

__int64 CAutoLive::GetThisVersion()
{
	__int64 i64Ver;
	TCHAR szFileName[MAX_PATH];
	if (::GetModuleFileName(_Module.m_hInstResource, szFileName, MAX_PATH))
	{
		DWORD dwHandle;
		int VSize = GetFileVersionInfoSize(szFileName, &dwHandle);

		if (VSize > 0) 
		{
			VS_FIXEDFILEINFO *pVi;
			BYTE *VersionBuffer = new BYTE[VSize];
			PWORD pVBuff = (PWORD)&i64Ver;
			if (GetFileVersionInfo(szFileName, dwHandle, VSize, VersionBuffer)) 
			{
				if (VerQueryValue(VersionBuffer, _T("\\"), (LPVOID *)&pVi, (PUINT)&VSize))
				{
					pVBuff[3] = HIWORD(pVi->dwFileVersionMS);
					pVBuff[2] = LOWORD(pVi->dwFileVersionMS);
					pVBuff[1] = HIWORD(pVi->dwFileVersionLS);
					pVBuff[0] = LOWORD(pVi->dwFileVersionLS);
				}
			}
			delete VersionBuffer;
		}
	}
	return i64Ver;
}

bool CAutoLive::getThisVersionString( stringt & verString )
{
	return GetVersionString(GetThisVersion(),verString);
}

__int64 CAutoLive::GetOnlineVersion()
{
	
	
	return 0;
}

__int64 CAutoLive::GetStringVersion( LPCTSTR pVersion )
{
	unsigned int v1,v2,v3,v4;
	_stscanf_s(pVersion,_T("%d.%d.%d.%d"),&v1,&v2,&v3,&v4);
	LARGE_INTEGER li;
	li.HighPart = MAKEWPARAM(v2,v1);
	li.LowPart = MAKEWPARAM(v4,v3);
	return li.QuadPart;
}

bool CAutoLive::GetVersionString( __int64 ver,stringt& verString )
{

	LARGE_INTEGER li;
	li.QuadPart = ver;

	WORD wV1 = HIWORD(li.HighPart);
	WORD wV2 = LOWORD(li.HighPart);
	WORD wV3 =  HIWORD(li.LowPart);
	WORD wV4 =  LOWORD(li.LowPart); 
	TCHAR szVerion[24];
	_stprintf_s(szVerion,_T("%d.%d.%d.%d"),wV1,wV2,wV3,wV4);
	verString = szVerion;
	return true;
}

bool CAutoLive::GetCurrentPageURL( stringt & URL )
{
	URL = m_pageURL;
	return true;
}

bool CAutoLive::HasNewVersion()
{
	return m_bHasNewVersion;
}

bool CAutoLive::getVersionExe( stringt& strExe )
{
	strExe = m_newVersionURL;
	return true;
}

void CAutoLive::update()
{
	HttpWinInet version;
	std::string iniString;
	stringt newVer,curVer;
	version.downFile(VERSION_URL);
	version.getFileString(iniString);
// 	iniString = "[VERSION]\n"
// 		"ver=1.0.0.1\n"
// 		"exe=http://youxi.baidu.com/zq/micro-client-v2/v.zip\n"
// 	"[1.0.0.1]\n"
// 	"page=http://youxi.baidu.com/hp/micro-client/\n"
// 	"bbs=http://youxi.baidu.com/bbs/\n"
// 	"charge=http://youxi.baidu.com/bbs/\n"
// 	"kefu=http://youxi.baidu.com/bbs/hp/\n"
// 	"offpage=http://youxi.baidu.com/hp/";

	CMemIni ini(_bstr_t(iniString.c_str()));
	ini.GetValue(VERSION_SECTION,VER_URL_KEY,newVer);
	ini.GetValue(VERSION_SECTION,EXE_URL_KEY,m_newVersionURL);

	__int64 newVerNum = GetStringVersion(newVer.c_str());
	__int64 thisVerNum = GetThisVersion();
	GetVersionString(thisVerNum,curVer);

	if(newVerNum>thisVerNum)
	{
		//需要提示	
		m_bHasNewVersion = true;
	}
	else
		m_bHasNewVersion = false;

	ini.GetValue(curVer.c_str(),PAGE_URL_KEY,m_pageURL);
	ini.GetValue(curVer.c_str(),BBS_URL_KEY,m_bbsURL);
	ini.GetValue(curVer.c_str(),CHARGE_URL_KEY,m_chargeURL);
	ini.GetValue(curVer.c_str(),KEFU_URL_KEY,m_kefuURL);
	ini.GetValue(curVer.c_str(),OFFPAGE_URL_KEY,m_offPageURL);
}

bool CAutoLive::GetBBSURL( stringt& bbsURL )
{
	bbsURL = m_bbsURL;
	return true;
}

bool CAutoLive::GetChargeURL( stringt& chargeURL )
{
	chargeURL = m_chargeURL;
	return true;
}

bool CAutoLive::GetKefuURL( stringt& keful )
{
	keful = m_kefuURL;
	return true;
}

bool CAutoLive::GetOffPageURL( stringt& OffPageURL )
{
	OffPageURL = m_offPageURL;
	return true;
}
