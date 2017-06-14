#include "StdAfx.h"
#include "resource.h"
#include "UpdateLayeredTool.h"
UpdateLayeredTool * UpdateLayeredTool::m_pInstance = NULL;
UpdateLayeredTool::UpdateLayeredTool()
{
	m_hFuncInst = LoadLibraryW(L"User32.DLL");
	if(m_hFuncInst)
	{
		Fun_UpdateLayeredWindow=(UPDATELAYER_FUNC)GetProcAddress(m_hFuncInst, "UpdateLayeredWindow");
		Fun_SetLayeredWindowAttributes = (SETLAYERATT_FUNC)GetProcAddress(m_hFuncInst, "SetLayeredWindowAttributes");
	}
}

UpdateLayeredTool & UpdateLayeredTool::instance()
{
	if(!m_pInstance)
	{
		m_pInstance = new UpdateLayeredTool();
	}
	return *m_pInstance;
}

UpdateLayeredTool::~UpdateLayeredTool()
{
	if(m_hFuncInst)
		FreeLibrary(m_hFuncInst);
}

void UpdateLayeredTool::release()
{
	if(m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

BOOL UpdateLayeredTool::SetLayeredWindowAttributes( HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags )
{
	if(Fun_SetLayeredWindowAttributes)
		return Fun_SetLayeredWindowAttributes(hwnd,crKey,bAlpha,dwFlags);
	return FALSE;
}

BOOL UpdateLayeredTool::UpdateLayeredWindow( HWND hWnd,HDC hdcDst,POINT *pptDst,SIZE *psize,HDC hdcSrc,POINT *pptSrc,COLORREF crKey,BLENDFUNCTION *pblend,DWORD dwFlags )
{
	if(Fun_UpdateLayeredWindow)
		return Fun_UpdateLayeredWindow(hWnd,hdcDst,pptDst,psize,hdcSrc,pptSrc,crKey,pblend,dwFlags);
	return FALSE;
}





UpdateLayered::UpdateLayered(HWND hTransWnd)
{
	m_hWnd = hTransWnd;
}

UpdateLayered::~UpdateLayered(void)
{
}

void UpdateLayered::UpdateLayer()
{
	if(m_bkImage.GetPngImage()==NULL)
		return;
	if(!IsWindow(m_hWnd))
		return ;

	HDC hDC = GetDC(m_hWnd);
	HDC hMemDC = NULL;
	HBITMAP hBitMap = NULL;
	do 
	{
		RECT rcWnd;
		GetWindowRect(m_hWnd,&rcWnd);
		POINT ptWinPos = {rcWnd.left, rcWnd.top};

		hMemDC = CreateCompatibleDC(hDC);
		int nBakWidth =  m_bkImage.getWidth();
		int nBakHeight = m_bkImage.getHeight();
		hBitMap =  CreateCompatibleBitmap(hDC, nBakWidth, nBakHeight);
		SelectObject(hMemDC, hBitMap);
		Gdiplus::Graphics graphic(hMemDC);
		
		Gdiplus::Point points[] = {Gdiplus::Point(0,0), Gdiplus::Point(nBakWidth,0), Gdiplus::Point(0,nBakHeight)};
		graphic.DrawImage(m_bkImage, points, 3);

		SIZE sizeWindow={nBakWidth, nBakHeight};
		POINT ptSrc = {0,0};

		DWORD dwExStyle = GetWindowLong(m_hWnd,GWL_EXSTYLE);
		if(!(dwExStyle & WS_EX_LAYERED))
		{
			SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle | WS_EX_LAYERED);
		}
		BLENDFUNCTION blendFunction;

		blendFunction.BlendOp=0;
		blendFunction.BlendFlags=0;
		blendFunction.AlphaFormat=1;
		blendFunction.SourceConstantAlpha=255;

		UpdateLayeredTool::instance().UpdateLayeredWindow(m_hWnd, hDC, &ptWinPos, &sizeWindow, hMemDC, &ptSrc, 0, &blendFunction, 2);

		graphic.ReleaseHDC(hMemDC);
	} while (0);

	if(hBitMap)
		DeleteObject(hBitMap);

	if(hMemDC)
		DeleteDC(hMemDC);
	
	ReleaseDC(m_hWnd,hDC);

}

void UpdateLayered::createColorTransparent(COLORREF clr)
{
	DWORD dwExStyle = GetWindowLong(m_hWnd,GWL_EXSTYLE);
	if(!(dwExStyle & WS_EX_LAYERED))
	{
		SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle | WS_EX_LAYERED);
	}

	UpdateLayeredTool::instance().SetLayeredWindowAttributes(m_hWnd,clr,255,LWA_COLORKEY|LWA_ALPHA);
}



bool UpdateLayered::loadBackground(UINT resID,HINSTANCE hInstRes)
{
	if(!m_bkImage.Load(resID,L"PNG",hInstRes))
		return false;
	return true;
}

SIZE UpdateLayered::getBkImageSize()
{
	int nBakWidth =  m_bkImage.getWidth();
	int nBakHeight = m_bkImage.getHeight();
	SIZE sizeWindow={nBakWidth, nBakHeight};
	return sizeWindow;
}

HWND UpdateLayered::attach( HWND hWnd )
{
	HWND hOldWnd = m_hWnd;
	m_hWnd = hWnd;
	return hOldWnd;
}
