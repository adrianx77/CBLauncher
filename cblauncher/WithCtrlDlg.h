// WithCtrlDlg.h : interface of the CWithCtrlDlg class
//
/////////////////////////////////////////////////////////////////////////////
#include "GdiPlusPngResource.h"
#include "UpdateLayeredTool.h"
#include "AtlBitmapButton.h"
#include "resource.h"
#include "FlashDlg.h"
#include "AutoLive.h"
#pragma once
#define WM_NAVIGATEPAGE WM_USER + 100
#define WM_GETURL	WM_USER+101
#define WM_NEWVERSION WM_USER + 102

class CBackGroundDlg;
class CWithCtrlDlg : public CAxDialogImpl<CWithCtrlDlg>,
	public IDispEventImpl<IDC_EXPLORER1,CWithCtrlDlg>
{
public:
	CAtlBitmapButton m_btnMainPage;
	CAtlBitmapButton m_btnCharge;
	CAtlBitmapButton m_btnBBS;
	CAtlBitmapButton m_btnKeFu;
	CAtlBitmapButton m_btnMinisize;
	CAtlBitmapButton m_btnClose;

	UpdateLayered m_upLayered;

	CBackGroundDlg & m_backDlg;
	CFlashDlg m_FlashDlg;
	CBrush m_brush;
	COLORREF m_clrTrans;

	BOOL m_bPlaying;
	CAutoLive m_live;

	enum { IDD = IDD_DLG_MAX };

	BEGIN_MSG_MAP(CWithCtrlDlg)
		MESSAGE_HANDLER(WM_NCHITTEST,OnNCHitTest)
		MESSAGE_HANDLER(WM_MOVE,OnMove)
		MESSAGE_HANDLER(WM_DESTROY,OnDestroy)

		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORDLG,OnCtrlColorDlg)
		MESSAGE_HANDLER(WM_NAVIGATEPAGE, OnNavigatePage)
		MESSAGE_HANDLER(WM_GETURL,OnGetURL)
		MESSAGE_HANDLER(WM_NEWVERSION,OnNewVersion);

		COMMAND_HANDLER(IDC_BTN_CLOSE, BN_CLICKED, OnBnClickedBtnClose)
		COMMAND_HANDLER(IDC_BTN_MINBOX, BN_CLICKED, OnBnClickedBtnMinbox)
		COMMAND_HANDLER(IDC_BTN_BBS, BN_CLICKED, OnBnClickedBtnBbs)
		COMMAND_HANDLER(IDC_BTN_OFFPAGE, BN_CLICKED, OnBnClickedBtnOffpage)
		COMMAND_HANDLER(IDC_BTN_CHARGE, BN_CLICKED, OnBnClickedBtnCharge)
		COMMAND_HANDLER(IDC_BTN_CUS_SERVICE, BN_CLICKED, OnBnClickedBtnCusService)

		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	CWithCtrlDlg(CBackGroundDlg&dlg);
	LRESULT OnCtrlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	HRESULT GetIEBrowser(IWebBrowser ** ppWB);
	LRESULT OnNavigatePage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnGetURL(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNewVersion(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	BEGIN_SINK_MAP(CWithCtrlDlg)
		SINK_ENTRY(IDC_EXPLORER1, 259, DocumentCompleteExplorer1)
		SINK_ENTRY(IDC_EXPLORER1, 252, NavigateComplete2Explorer1)
		SINK_ENTRY(IDC_EXPLORER1, 250, BeforeNavigate2Explorer1)
	END_SINK_MAP()

	void __stdcall DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL);
	void __stdcall NavigateComplete2Explorer1(LPDISPATCH pDisp, VARIANT* URL);
	void __stdcall BeforeNavigate2Explorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
	LRESULT OnBnClickedBtnClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnMinbox(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnBbs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnOffpage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnCharge(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnCusService(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	void layoutSubItem();
public:
	void showMainCtrl();
	void hideMainCtrl();
	void createGame();
	void enterGame(BSTR url);
	void Exit();
	void OpenURL(stringt & strURL);

};


