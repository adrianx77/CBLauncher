#include "StdAfx.h"
#include "GdiPlusPng.h"
CGdiPlusPng::CGdiPlusPng()  
{  
	m_pImage = NULL;  
}  

CGdiPlusPng::CGdiPlusPng(LPCWSTR pFile)  
{  
	m_pImage = NULL;  
	Load(pFile);  
}  

CGdiPlusPng::~CGdiPlusPng()  
{  
	Empty();  
}  

void CGdiPlusPng::Empty() 
{  
	delete m_pImage;  
	m_pImage = NULL;  
}  
bool CGdiPlusPng::Load(LPCWSTR pFile)  
{  
	Empty();  
	m_pImage = Gdiplus::Bitmap::FromFile(pFile);  
	return m_pImage->GetLastStatus() == Gdiplus::Ok;  
}  

Gdiplus::Image *CGdiPlusPng::GetPngImage() const  
{  
	return m_pImage;  
}  

int CGdiPlusPng::getWidth()
{
	if(!m_pImage)
		return 0;
	return m_pImage->GetWidth();
}

int CGdiPlusPng::getHeight()
{
	if(!m_pImage)
		return 0;
	return m_pImage->GetHeight();
}
