#pragma once
#include <wininet.h>

class HttpWinInet
{
	BYTE * m_bytContent;
	size_t m_BufSize ;
	HINTERNET m_hPreConfig;
	int m_nLen;

public:
	HttpWinInet(void);
	virtual ~HttpWinInet(void);
	int downFile(LPCTSTR pszURL);
	bool getFileString(std::string & str);
};
