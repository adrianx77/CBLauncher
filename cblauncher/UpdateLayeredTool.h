#pragma once
#include "GdiPlusPngResource.h"
class UpdateLayeredTool
{
private:
	HINSTANCE m_hFuncInst;
	typedef BOOL (WINAPI *UPDATELAYER_FUNC)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);
	typedef BOOL (WINAPI *SETLAYERATT_FUNC)(HWND,COLORREF,BYTE,DWORD);
	UPDATELAYER_FUNC Fun_UpdateLayeredWindow;
	SETLAYERATT_FUNC Fun_SetLayeredWindowAttributes;
	UpdateLayeredTool();
	static UpdateLayeredTool * m_pInstance;
public:
	static UpdateLayeredTool & instance();
	static void release();

	virtual ~UpdateLayeredTool();
	BOOL SetLayeredWindowAttributes( HWND hwnd,  COLORREF crKey,  BYTE bAlpha,  DWORD dwFlags);
	BOOL UpdateLayeredWindow(HWND hWnd,HDC hdcDst,POINT *pptDst,SIZE *psize,HDC hdcSrc,POINT *pptSrc,COLORREF crKey,BLENDFUNCTION *pblend,DWORD dwFlags);
};


class UpdateLayered
{
	CGdiPlusPngResource m_bkImage;
	HWND m_hWnd;
public:
	UpdateLayered(HWND hTransWnd = NULL);
	~UpdateLayered(void);
	HWND attach(HWND hWnd);
	void createColorTransparent(COLORREF clr);
	bool loadBackground(UINT resID,HINSTANCE hInstRes);
	void UpdateLayer();
	SIZE getBkImageSize();
};
