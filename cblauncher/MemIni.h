#pragma once



class CMemIni
{
	typedef std::map<stringt,stringt> KEY_VALUEINFO;
	typedef std::map<stringt,std::map<stringt,stringt> >  SECTIONINFO;
	SECTIONINFO m_sectionInfo;
	stringt _curSection;
	bool parse(LPCTSTR pStrings);
	bool process_line(LPCTSTR pString);
	bool process_kv(LPCTSTR section,LPCTSTR pString);
public:
	CMemIni(LPCTSTR pString);
	virtual ~CMemIni(void);
	size_t GetSectionCount();
	size_t GetSection(int indxSection,LPTSTR pSection);
	bool GetSection(int indxSection,stringt& strSection);
	size_t GetValue(LPCTSTR pSection,LPCTSTR pKey,LPTSTR pValue);
	bool GetValue(LPCTSTR pSection,LPCTSTR pKey,stringt& strValue);
};
