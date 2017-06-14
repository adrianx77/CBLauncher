#include "StdAfx.h"
#include "HttpWinInet.h"

HttpWinInet::HttpWinInet(void)
{
	m_hPreConfig = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
	m_BufSize = 1024 *1024;
	m_bytContent = new BYTE[m_BufSize];
}

HttpWinInet::~HttpWinInet(void)
{
	delete []m_bytContent;
	if(m_hPreConfig!=NULL)
		InternetCloseHandle(m_hPreConfig);
}

int HttpWinInet::downFile( LPCTSTR pszURL )
{
	if(m_hPreConfig==NULL)
		return 0;
	HINTERNET hCurrDown = InternetOpenUrl(m_hPreConfig,pszURL,NULL,NULL,INTERNET_FLAG_NO_CACHE_WRITE,NULL);
	DWORD dwReadDataLength = NULL;
	BOOL bRet = TRUE;
	int iCurLen=0;
	do 
	{
		bRet = InternetReadFile(hCurrDown,m_bytContent + iCurLen,m_BufSize - iCurLen,&dwReadDataLength);
		iCurLen += dwReadDataLength;
	} while (0 != dwReadDataLength && iCurLen<m_BufSize-1);
	m_bytContent[iCurLen]=0;
	m_nLen = iCurLen;
	return iCurLen;
}

bool HttpWinInet::getFileString(std::string & str )
{
	str = (LPCSTR)m_bytContent;
	return true;
}
