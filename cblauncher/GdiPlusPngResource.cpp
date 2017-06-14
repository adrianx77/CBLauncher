#include "StdAfx.h"
#include "GdiPlusPngResource.h"

void CGdiPlusPngResource::Empty()  
{  
	CGdiPlusPng::Empty();  
	if (m_hBuffer)  
	{  
		::GlobalUnlock(m_hBuffer);  
		::GlobalFree(m_hBuffer);  
		m_hBuffer = NULL;  
	}   
}  

bool CGdiPlusPngResource::Load(LPCTSTR pName, LPCTSTR pType, HMODULE hInst)  
{  
	Empty();  

	HRSRC hResource = ::FindResource(hInst, pName, pType);  
	if(hResource == NULL)  
	{  
		return false;  
	}  

	DWORD imageSize = ::SizeofResource(hInst, hResource);  
	if(imageSize == 0)  
	{  
		return false;  
	}  

	const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));  
	if(pResourceData == NULL)  
	{  
		return false;  
	}  

	m_hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);  
	if(m_hBuffer != NULL)  
	{  
		void* pBuffer = ::GlobalLock(m_hBuffer);  
		if(pBuffer != NULL)  
		{  
			CopyMemory(pBuffer, pResourceData, imageSize);  

			IStream* pStream = NULL;  
			if(::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)  
			{  
				m_pImage = Gdiplus::Image::FromStream(pStream);  
				pStream->Release();  
				if(m_pImage != NULL)  
				{   
					if(m_pImage->GetLastStatus() == Gdiplus::Ok)  
					{  
						return true;  
					}  

					delete m_pImage;  
					m_pImage = NULL;  
				}  
			}  
			::GlobalUnlock(m_hBuffer);  
		}  
		::GlobalFree(m_hBuffer);  
		m_hBuffer = NULL;  
	}  
	return false;  
}  