#include "StdAfx.h"
#include "MemIni.h"

CMemIni::CMemIni(LPCTSTR pString)
{
	parse(pString);
}

CMemIni::~CMemIni(void)
{
}
LPTSTR getValidateString(LPTSTR pF1,LPTSTR pF2)
{
	LPTSTR pF = pF1;
	if(!pF)
	{
		return pF2;
	}
	if(!pF2)
		return pF1;
	return min(pF2,pF1);
}

bool CMemIni::parse( LPCTSTR pStrings )
{
	LPTSTR pCurFirst = (LPTSTR)pStrings;
	LPTSTR pFind1 = StrChrI(pStrings,'\n');
	LPTSTR pFind2 = StrChrI(pStrings,'\r');
	LPTSTR pFind = getValidateString(pFind1,pFind2);
	while (pFind)
	{
		stringt s(pCurFirst,pFind);
		process_line(s.c_str());
		pCurFirst = pFind;
		if(pCurFirst==NULL)
			break;
		
		while ((*pCurFirst=='\n' || *pCurFirst=='\r'))
		{
			pCurFirst ++ ;
		}
		pFind1 = StrChrI(pCurFirst,'\n');
		pFind2 = StrChrI(pCurFirst,'\r');
		pFind = getValidateString(pFind1,pFind2);
	}
	if(pFind==NULL)
	{
		process_line(pCurFirst);
	}	
	return true;
}

bool CMemIni::process_line( LPCTSTR pString )
{
	stringt newLine = pString;
	if(newLine.size()==0)
		return false;

	while((*newLine.rbegin() == '\r') || (*newLine.rbegin()=='\n'))
	{
		newLine.erase(newLine.end()-1);
	}
	
	
	//process key value
	if(newLine[0]=='[')
	{
		_curSection = newLine.substr(1,newLine.length()-2);
	}
	else
	{
		//process section
		return process_kv(_curSection.c_str(),pString);
		
	}
	return true;
}

bool CMemIni::process_kv( LPCTSTR section,LPCTSTR pString )
{
	if(!section)
		return false;
	if(!pString)
		return false;

	LPTSTR pFind = StrChr(pString,'=');
	if(!pFind)
		return false;

	stringt sK(pString,pFind);
	stringt sV(pFind+1);
	stringt sec(section);
	if(m_sectionInfo.find(sec)!=m_sectionInfo.end())
	{
		m_sectionInfo[sec][sK] = sV;
	}
	else
	{
		std::map<stringt,stringt> _map;
		_map[sK]=sV;
		m_sectionInfo[sec] = _map;
	}
	return true;
}

size_t CMemIni::GetSectionCount()
{
	return m_sectionInfo.size();
}

size_t CMemIni::GetSection( int indxSection,LPTSTR pSection )
{
	int idx = 0;
	for(SECTIONINFO::iterator it = m_sectionInfo.begin();it!=m_sectionInfo.end();++it)
	{
		if(indxSection==idx)
		{
			if(pSection==NULL)
				return (*it).first.length();

			StrCpy(pSection,(*it).first.c_str());
			return (*it).first.length();
		}
		idx++;
	}
	return 0;
	
}

bool CMemIni::GetSection( int indxSection,stringt& strSection )
{
	int idx = 0;
	for(SECTIONINFO::iterator it = m_sectionInfo.begin();it!=m_sectionInfo.end();++it)
	{
		if(indxSection==idx)
		{
			strSection = (*it).first.c_str();
			return true;
		}
		idx++;
	}
	return false;
}

size_t CMemIni::GetValue( LPCTSTR pSection,LPCTSTR pKey,LPTSTR pValue )
{
	SECTIONINFO::iterator it = m_sectionInfo.find(pSection);	
	if(it==m_sectionInfo.end())
	{
		return 0;
	}
	KEY_VALUEINFO::iterator kvIt = (*it).second.find(pKey);
	if(kvIt==(*it).second.end())
	{
		return 0;
	}
	if(pValue==NULL)
		return (*kvIt).second.length();

	StrCpy(pValue,(*kvIt).second.c_str());
	return (*kvIt).second.length();
}

bool CMemIni::GetValue( LPCTSTR pSection,LPCTSTR pKey,stringt& strValue )
{
	SECTIONINFO::iterator it = m_sectionInfo.find(pSection);	
	if(it==m_sectionInfo.end())
	{
		return false;
	}
	KEY_VALUEINFO::iterator kvIt = (*it).second.find(pKey);
	if(kvIt==(*it).second.end())
	{
		return false;
	}
	strValue = (*kvIt).second.c_str();
	return true;
}

