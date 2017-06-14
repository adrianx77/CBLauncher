// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#include "GdiPlusPngResource.h"
#include "UpdateLayeredTool.h"
#include "WithCtrlDlg.h"
#pragma once

class CBackGroundDlg : public CAxDialogImpl<CBackGroundDlg>, 
				       public CUpdateUI<CBackGroundDlg>,
					   public CMessageFilter, public CIdleHandler
{
protected:
	CWithCtrlDlg m_WithCtrlDlg;
	BOOL m_bCanBeHit;
	UpdateLayered m_upLayered;
public:
	CBackGroundDlg();

	enum { IDD = IDD_MAINDLG };
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(WM_RBUTTONUP ==pMsg->message 
			|| WM_RBUTTONDOWN == pMsg->message
			|| WM_RBUTTONDBLCLK == pMsg->message)
		{
			return TRUE;
		}
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CBackGroundDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CBackGroundDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_NCHITTEST,OnNCHitTest)
		MESSAGE_HANDLER(WM_MOVE,OnMove)
		MESSAGE_HANDLER(WM_TIMER,OnTimer)
		MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()


	BOOL CanBeHit();
	void CancelBeHit();
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void CloseDialog(int nVal);
	void createShortcutDesktop(LPCTSTR pszExe);
};
