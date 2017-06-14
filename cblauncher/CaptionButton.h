// CaptionButton.h : interface of the CCaptionButton  class
//
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"
#include <vector>
#pragma once
#define CUSTOMCAPTIONPTR _T("CustomCaptionSubclassPtr")
template<class T>
class CCaptionButton
{
	typedef struct
	{
		UINT	uCmd;			//command to send when clicked
		int		nRightBorder;	//Pixels between this button and buttons to the right
		HBITMAP hBmp;			//Bitmap to display
		BOOL	fPressed;		//Private.
	} CapBtnData;
	typedef std::vector<CapBtnData> CAPBTNDATALIST;

public:
	enum{B_EDGE=2};
	CAPBTNDATALIST m_buttons;
	BOOL		m_fMouseDown;
	WNDPROC		m_wpOldProc;
	int			m_iActiveButton;
	CCaptionButton()
	{
		m_bSetProp = FALSE;
	}
	virtual ~CCaptionButton()
	{
		m_buttons.clear();
	}

	int CalcTopEdge()
	{
		DWORD dwStyle = 0;
		T * pThis = static_cast<T*> (this);
		if(pThis && IsWindow(pThis->m_hWnd))
		{			
			dwStyle = GetWindowLong(pThis->m_hWnd, GWL_STYLE);
		}
		if(dwStyle & WS_THICKFRAME)
			return GetSystemMetrics(SM_CYSIZEFRAME);
		else
			return GetSystemMetrics(SM_CYFIXEDFRAME);
	}
	int CalcRightEdge()
	{
		DWORD dwStyle = 0;
		T * pThis = static_cast<T*> (this);
		if(pThis && IsWindow(pThis->m_hWnd))
		{			
			dwStyle = GetWindowLong(pThis->m_hWnd, GWL_STYLE);
		}

		if(dwStyle & WS_THICKFRAME)
			return GetSystemMetrics(SM_CXSIZEFRAME);
		else
			return GetSystemMetrics(SM_CXFIXEDFRAME);

	}
	int GetRightEdgeOffset()
	{
		DWORD dwStyle = 0;
		DWORD dwExStyle = 0;

		T * pThis = static_cast<T*> (this);
		if(pThis && IsWindow(pThis->m_hWnd))
		{			
			dwStyle = GetWindowLong(pThis->m_hWnd, GWL_STYLE);
			dwExStyle = GetWindowLong(pThis->m_hWnd, GWL_EXSTYLE);
		}	

		int nButSize = 0;
		int nSysButSize;
		
		if(dwExStyle & WS_EX_TOOLWINDOW)
		{
			nSysButSize = GetSystemMetrics(SM_CXSMSIZE) - B_EDGE;

			if(dwStyle & WS_SYSMENU)	
				nButSize += nSysButSize + B_EDGE;

			return nButSize + CalcRightEdge();
		}
		else
		{
			nSysButSize = GetSystemMetrics(SM_CXSIZE) - B_EDGE;

			// Window has Close [X] button. This button has a 2-pixel
			// border on either size
			if(dwStyle & WS_SYSMENU)	
			{
				nButSize += nSysButSize + B_EDGE;
			}

			// If either of the minimize or maximize buttons are shown,
			// Then both will appear (but may be disabled)
			// This button pair has a 2 pixel border on the left
			if(dwStyle & (WS_MINIMIZEBOX | WS_MAXIMIZEBOX) )
			{
				nButSize += B_EDGE + nSysButSize * 2;
			}
			// A window can have a question-mark button, but only
			// if it doesn't have any min/max buttons
			else if(dwExStyle & WS_EX_CONTEXTHELP)
			{
				nButSize += B_EDGE + nSysButSize;
			}

			// Now calculate the size of the border...aggghh!
			return nButSize + CalcRightEdge();
		}
	}
	void GetButtonRect(int idx, RECT *rect, BOOL fWindowRelative)
	{
		T * pThis = static_cast<T*> (this);
		if(!pThis || !IsWindow(pThis->m_hWnd))
		{
			SetRectEmpty(rect);
			return;
		}
		int i, re_start;
		int cxBut, cyBut;

		if(GetWindowLong(pThis->m_hWnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)
		{
			cxBut = GetSystemMetrics(SM_CXSMSIZE);
			cyBut = GetSystemMetrics(SM_CYSMSIZE);
		}
		else
		{
			cxBut = GetSystemMetrics(SM_CXSIZE);
			cyBut = GetSystemMetrics(SM_CYSIZE);
		}

		// right-edge starting point of inserted buttons
		re_start = GetRightEdgeOffset();

		GetWindowRect(pThis->m_hWnd, rect);

		if(fWindowRelative)
			OffsetRect(rect, -rect->left, -rect->top);

		//Find the correct button - but take into
		//account all other buttons.
		for(i = 0; i <= idx; i++)
		{
			re_start += m_buttons[i].nRightBorder + cxBut - B_EDGE;
		}

		rect->left   = rect->right  - re_start;
		rect->top    = rect->top  + CalcTopEdge() +  B_EDGE;
		rect->right  = rect->left + cxBut - B_EDGE;
		rect->bottom = rect->top  + cyBut - B_EDGE*2;
	}
	LRESULT Caption_NCHitTest(WPARAM wParam, LPARAM lParam)
	{
		T * pThis = static_cast<T*> (this);
		if(!pThis || !IsWindow(pThis->m_hWnd))
		{
			return 0;
		}

		RECT  rect;
		POINT pt;
		UINT  ret;

		pt.x = (short)LOWORD(lParam);
		pt.y = (short)HIWORD(lParam);

		ret = CallWindowProc(m_wpOldProc, pThis->m_hWnd, WM_NCHITTEST, wParam, lParam);

		//If the mouse is in the caption, then check to
		//see if it is over one of our buttons
		if(ret == HTCAPTION)
		{
			for(UINT   i = 0; i < m_buttons.size(); i++)
			{
				GetButtonRect(i, &rect, FALSE);
				InflateRect(&rect, 0, B_EDGE);

				//If the mouse is in any custom button, then
				//We need to override the default behaviour.
				if(PtInRect(&rect, pt))
				{
					return HTBORDER;
				}
			}
		}

		return ret;
	}

	void CenterBitmap(HDC hdc, RECT *rc, HBITMAP hBitmap)
	{
		BITMAP bm;
		int cx;
		int cy;   
		HDC memdc;
		HBITMAP hOldBM;
		RECT  rcDest = *rc;   
		POINT p;
		SIZE  delta;
		COLORREF colorOld;

		if(hBitmap == NULL) 
			return;

		// center bitmap in caller's rectangle   
		GetObject(hBitmap, sizeof bm, &bm);   

		cx = bm.bmWidth;
		cy = bm.bmHeight;

		delta.cx = (rc->right-rc->left - cx) / 2;
		delta.cy = (rc->bottom-rc->top - cy) / 2;

		if(rc->right-rc->left > cx)
		{
			SetRect(&rcDest, rc->left+delta.cx, rc->top + delta.cy, 0, 0);   
			rcDest.right = rcDest.left + cx;
			rcDest.bottom = rcDest.top + cy;
			p.x = 0;
			p.y = 0;
		}
		else
		{
			p.x = -delta.cx;   
			p.y = -delta.cy;
		}

		// select checkmark into memory DC
		memdc = CreateCompatibleDC(hdc);
		hOldBM = (HBITMAP)SelectObject(memdc, hBitmap);

		// set BG color based on selected state   
		colorOld = SetBkColor(hdc, GetSysColor(COLOR_3DFACE));

		BitBlt(hdc, rcDest.left, rcDest.top, rcDest.right-rcDest.left, rcDest.bottom-rcDest.top, memdc, p.x, p.y, SRCCOPY);

		// restore
		SetBkColor(hdc, colorOld);
		SelectObject(memdc, hOldBM);
		DeleteDC(memdc);

	}
	LRESULT Caption_NCPaint(HRGN hrgn)
	{
		T * pThis = static_cast<T*> (this);
		if(!pThis || !IsWindow(pThis->m_hWnd))
		{
			return 0;
		}

		RECT rect, rect1;
		BOOL fRegionOwner = FALSE;
		HDC hdc;
		UINT uButType;

		int x, y;

		HRGN hrgn1;

		GetWindowRect(pThis->m_hWnd, &rect);

		x = rect.left;
		y = rect.top;

		//Create a region which covers the whole window. This
		//must be in screen coordinates
		if(hrgn == (HRGN)1 || hrgn == 0)
		{
			hrgn = CreateRectRgnIndirect(&rect);
			fRegionOwner = TRUE;
		}

		// Clip our custom buttons out of the way...
		for(UINT i = 0; i < m_buttons.size(); i++)
		{
			//Get button rectangle in screen coords
			GetButtonRect( i, &rect1, FALSE);

			hrgn1 = CreateRectRgnIndirect(&rect1);

			//Cut out a button-shaped hole
			CombineRgn(hrgn, hrgn, hrgn1, RGN_XOR);

			DeleteObject(hrgn1);
		}

		//
		//	Call the default window procedure with our modified window region!
		//	(REGION MUST BE IN SCREEN COORDINATES)
		//
		CallWindowProc(m_wpOldProc, pThis->m_hWnd, WM_NCPAINT, (WPARAM)hrgn, 0);

		//
		//	Now paint our custom buttons in the holes that are
		//	left by our clipping. All drawing in the Non-client area
		//  is window-relative (Not in screen coords)
		//

		hdc = GetWindowDC(pThis->m_hWnd);

		// Draw buttons in a loop
		for(UINT i = 0; i < m_buttons.size(); i++)
		{
			//Get Button rect in window coords
			GetButtonRect(i, &rect1, TRUE);

			if(m_buttons[i].hBmp)
				uButType = DFCS_BUTTONPUSH;
			else
				uButType = DFCS_BUTTONPUSH;

			if(m_buttons[i].fPressed)
				DrawFrameControl(hdc, &rect1, DFC_BUTTON,  uButType | DFCS_PUSHED);
			//DrawFrameControl(hdc, &rect1, DFC_CAPTION, DFCS_CAPTIONRESTORE | DFCS_PUSHED);
			else
				DrawFrameControl(hdc, &rect1, DFC_BUTTON, uButType);
			//DrawFrameControl(hdc, &rect1, DFC_CAPTION, DFCS_CAPTIONRESTORE);

			//Now draw the bitmap!
			InflateRect(&rect1, -2, -2);
			rect1.right--;
			rect1.bottom--;

			if(m_buttons[i].fPressed)
				OffsetRect(&rect1, 1, 1);

			CenterBitmap(hdc, &rect1, m_buttons[i].hBmp);
		}

		ReleaseDC(pThis->m_hWnd, hdc);

		if(fRegionOwner)
			DeleteObject(hrgn);

		return 0;
	}
	LRESULT Caption_Wrapper( UINT msg, WPARAM wParam, LPARAM lParam)
	{
		T * pThis = static_cast<T*> (this);
		if(!pThis || !IsWindow(pThis->m_hWnd))
		{
			return 0;
		}

		UINT ret;
		DWORD dwStyle;

		dwStyle = GetWindowLong(pThis->m_hWnd, GWL_STYLE);

		//Turn OFF WS_VISIBLE, so that WM_NCACTIVATE does not
		//paint our window caption...
		SetWindowLong(pThis->m_hWnd, GWL_STYLE, dwStyle & ~WS_VISIBLE);

		//Do the default thing..
		ret = CallWindowProc(m_wpOldProc, pThis->m_hWnd, msg, wParam, lParam);

		//Restore the original style
		SetWindowLong(pThis->m_hWnd, GWL_STYLE, dwStyle);

		//Paint the whole window frame + caption
		Caption_NCPaint((HRGN)1);
		return ret;
	}

	LRESULT Caption_NCActivate(WPARAM wParam, LPARAM lParam)
	{
		return Caption_Wrapper(WM_NCACTIVATE, wParam, lParam);
	}

	LRESULT Caption_SetText(WPARAM wParam, LPARAM lParam)
	{
		return Caption_Wrapper(WM_SETTEXT, wParam, lParam);
	}
	LRESULT Caption_NCLButtonDown(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		T * pThis = static_cast<T*> (this);
		if(!pThis || !IsWindow(pThis->m_hWnd))
		{
			return 0;
		}

		RECT rect;
		POINT pt;

		pt.x = (short)LOWORD(lParam);
		pt.y = (short)HIWORD(lParam);

		//If mouse has been clicked in caption
		//(Note: the NCHITTEST handler changes HTCAPTION to HTBORDER
		//       if we are over an inserted button)
		if(wParam == HTBORDER)
		{
			for(UINT i = 0; i < m_buttons.size(); i++)
			{
				GetButtonRect(i, &rect, FALSE);
				InflateRect(&rect, 0, 2);

				//if clicked in a custom button
				if(PtInRect(&rect, pt))
				{
					m_iActiveButton = i;
					m_buttons[i].fPressed = TRUE;
					m_fMouseDown = TRUE;

					SetCapture(pThis->m_hWnd);

					RedrawNC();

					return 0;
				}
			}
		}

		return CallWindowProc(m_wpOldProc, pThis->m_hWnd, msg, wParam, lParam);
	}
	LRESULT Caption_LButtonUp(WPARAM wParam, LPARAM lParam)
	{
		T * pThis = static_cast<T*> (this);
		if(!pThis || !IsWindow(pThis->m_hWnd))
		{
			return 0;
		}
		RECT rect;
		POINT pt;

		pt.x = (short)LOWORD(lParam);
		pt.y = (short)HIWORD(lParam);
		ClientToScreen(pThis->m_hWnd, &pt);

		if(m_fMouseDown)
		{
			ReleaseCapture();

			GetButtonRect(m_iActiveButton, &rect, FALSE);
			InflateRect(&rect, 0, 2);

			//if clicked in a custom button
			if(PtInRect(&rect, pt))
			{
				UINT uCmd = m_buttons[m_iActiveButton].uCmd;
				SendMessage(pThis->m_hWnd, WM_COMMAND, uCmd, MAKELPARAM(pt.x, pt.y));
			}

			m_buttons[m_iActiveButton].fPressed = FALSE;
			m_fMouseDown = FALSE;

			RedrawNC();

			return 0;
		}

		return CallWindowProc(pThis->m_wpOldProc, pThis->m_hWnd, WM_LBUTTONUP, wParam, lParam);
	}

	LRESULT Caption_MouseMove(WPARAM wParam, LPARAM lParam)
	{
		T * pThis = static_cast<T*> (this);
		if(!pThis || !IsWindow(pThis->m_hWnd))
		{
			return 0;
		}
		RECT rect;
		POINT pt;
		BOOL fPressed;

		pt.x = (short)LOWORD(lParam);
		pt.y = (short)HIWORD(lParam);
		ClientToScreen(pThis->m_hWnd, &pt);

		if(m_fMouseDown)
		{
			GetButtonRect(m_iActiveButton, &rect, FALSE);
			InflateRect(&rect, 0, 2);

			fPressed = PtInRect(&rect, pt);

			if(fPressed != m_buttons[m_iActiveButton].fPressed)
			{
				m_buttons[m_iActiveButton].fPressed ^= 1;
				RedrawNC();
			}

			return 0;
		}

		return CallWindowProc(m_wpOldProc, pThis->m_hWnd, WM_MOUSEMOVE, wParam, lParam);
	}

	//Replacement window procedure
	static LRESULT CALLBACK NewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		T * pThis = GetThis(hwnd);
		CCaptionButton * pBtn = static_cast<CCaptionButton*> (pThis);
		WNDPROC oldproc = pBtn->m_wpOldProc;

		switch(msg)
		{
			//clean up when window is destroyed
		case WM_NCHITTEST:
			return pBtn->Caption_NCHitTest(wParam, lParam);

			//These three messages all cause the caption to
			//be repainted, so we have to handle all three to properly
			//support inserted buttons
		case WM_NCACTIVATE:
			return pBtn->Caption_NCActivate(wParam, lParam);

		case WM_SETTEXT:
			return pBtn->Caption_SetText(wParam, lParam);

		case WM_NCPAINT:
			return pBtn->Caption_NCPaint((HRGN)wParam);

			//Mouse support
		case WM_NCLBUTTONDBLCLK:
		case WM_NCLBUTTONDOWN:
			return pBtn->Caption_NCLButtonDown(msg, wParam, lParam);

		case WM_LBUTTONUP:
			return pBtn->Caption_LButtonUp(wParam, lParam);

		case WM_MOUSEMOVE:
			return pBtn->Caption_MouseMove(wParam, lParam);
		}

		//call the old window procedure
		return CallWindowProc(oldproc, hwnd, msg, wParam, lParam);
	}

	void RedrawNC()
	{
		T * pThis = static_cast<T*> (this);
		if(!pThis || !IsWindow(pThis->m_hWnd))
		{
			return;
		}
		
		SetWindowPos(pThis->m_hWnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_DRAWFRAME);
	}
	BOOL ModifyButton(UINT uCmd,HBITMAP hBitmap)
	{
		T * pThis = static_cast<T*> (this);
		if(!pThis || !IsWindow(pThis->m_hWnd))
		{
			return 0;
		}
		for(UINT i = 0; i < m_buttons.size(); i++)
		{
			if(m_buttons[i].uCmd==uCmd)
			{
				m_buttons[i].hBmp = hBitmap;
				RedrawNC();
				return TRUE;
			}
		}

		return FALSE;
	}
	BOOL InsertButton(UINT uCmd, int nBorder, HBITMAP hBmp)
	{
		T * pThis = static_cast<T*> (this);
		if(!pThis || !IsWindow(pThis->m_hWnd))
		{
			return 0;
		}
		// If this window doesn't have any buttons yet,
		// then perform the subclass and allocate structures etc. 
		if(!m_bSetProp)
		{

			//allocate memory for our subclass information
			m_fMouseDown   = FALSE;

			//assign this to the window in question
			SetProp(pThis->m_hWnd, CUSTOMCAPTIONPTR, (HANDLE)pThis);

			//subclass the window
			m_wpOldProc = (WNDPROC)SetWindowLong(pThis->m_hWnd, GWL_WNDPROC, (LONG)NewWndProc);

			m_bSetProp = TRUE;
		}
		CapBtnData btn;
		
		btn.hBmp		= hBmp;
		btn.nRightBorder= nBorder;
		btn.uCmd		= uCmd;
		btn.fPressed    = FALSE;
		m_buttons.push_back(btn);
		return TRUE;
	}
	BOOL RemoveButton(int idx)
	{
		T * pThis = static_cast<T*> (this);
		if(!pThis || !IsWindow(pThis->m_hWnd))
		{
			return FALSE;
		}			
		
		if(idx>=m_buttons.size())
			return FALSE;
		
		m_buttons.erase(m_buttons.begin()+idx);

		RedrawNC();
	}
	void CaptionClear()
	{
		m_bSetProp = FALSE;
		m_buttons.clear();
	}
	static T *GetThis(HWND hwnd)
	{
		return (T *)GetProp(hwnd, CUSTOMCAPTIONPTR);
	}

	BOOL m_bSetProp;
};