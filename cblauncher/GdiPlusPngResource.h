#pragma once
#include "gdipluspng.h"

class CGdiPlusPngResource :
	public CGdiPlusPng
{
protected:  
	HGLOBAL m_hBuffer;  

public:  
	CGdiPlusPngResource()  
	{  
		m_hBuffer = NULL;  
	}  

	CGdiPlusPngResource(LPCTSTR pName, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL)  
	{  
		m_hBuffer = NULL;  
		Load(pName, pType, hInst);  
	}  

	CGdiPlusPngResource(UINT id, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL)  
	{  
		m_hBuffer = NULL;  
		Load(id, pType, hInst);  
	}  
	CGdiPlusPngResource(UINT id, UINT type, HMODULE hInst = NULL)  
	{  
		m_hBuffer = NULL;  
		Load(id, type, hInst);  
	}  

	virtual ~CGdiPlusPngResource()  
	{  
		Empty();  
	}  

	void Empty();  

	bool Load(LPCTSTR pName, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL);  

	bool Load(UINT id, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL)  
	{  
		return Load(MAKEINTRESOURCE(id), pType, hInst);  
	}  

	bool Load(UINT id, UINT type, HMODULE hInst = NULL)  
	{  
		return Load(MAKEINTRESOURCE(id), MAKEINTRESOURCE(type), hInst);  
	}  
};
