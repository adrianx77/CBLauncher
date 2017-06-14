#include "stdafx.h"
#include "HttpWinInet.h"
#include "WithCtrlDlg.h"
#include "BackGroundDlg.h"

#define REDIR_PREFIX _T("app://loadgame|")

CWithCtrlDlg::CWithCtrlDlg(CBackGroundDlg&dlg):m_backDlg(dlg),m_FlashDlg(*this)
{
	m_bPlaying = FALSE;
	m_clrTrans= RGB(255,255,0);
}
LRESULT CWithCtrlDlg::OnCtrlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return (LRESULT)(HBRUSH)m_brush;
}
LRESULT CWithCtrlDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	
	layoutSubItem();

	m_upLayered.attach(m_hWnd);
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);
	m_brush.CreateSolidBrush(m_clrTrans);
	m_upLayered.createColorTransparent(m_clrTrans);
	AtlAdviseSinkMap ( this, true );

	PostMessage(WM_GETURL,0,0);

	return TRUE;
}


LRESULT CWithCtrlDlg::OnNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_backDlg.CancelBeHit();
	bHandled = true;
	return HTCAPTION;
}
LRESULT CWithCtrlDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(::IsWindow(m_hWnd))
	{
		if(!m_backDlg.CanBeHit())
		{
			RECT rcBackDlg,rcWithCtrl;
			m_backDlg.GetWindowRect(&rcBackDlg);
			GetWindowRect(&rcWithCtrl);
			m_backDlg.MoveWindow(rcWithCtrl.left,rcWithCtrl.top,rcBackDlg.right-rcBackDlg.left,rcBackDlg.bottom-rcBackDlg.top,TRUE);
		}
	}
	bHandled = true;
	return 0;
}
LRESULT CWithCtrlDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	AtlAdviseSinkMap ( this, false );
	return 0;
}
LRESULT CWithCtrlDlg::OnBnClickedBtnClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	m_backDlg.CloseDialog(wID);
	m_FlashDlg.DestroyWindow();
	return 0;
}

HRESULT CWithCtrlDlg::GetIEBrowser( IWebBrowser ** ppWB )
{
	CAxWindow   axWeb   =   GetDlgItem(IDC_EXPLORER1); 
	return axWeb.QueryControl(ppWB); 
}

void __stdcall CWithCtrlDlg::DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	// TODO: 在此处添加消息处理程序代码
	stringt curPage;
	m_live.GetCurrentPageURL(curPage);
	if(0==StrCmpI((LPCTSTR)_bstr_t(curPage.c_str()),URL->bstrVal))
	{
		CAxWindow   axWeb   =   GetDlgItem(IDC_EXPLORER1); 
		axWeb.ShowWindow(SW_SHOW);
		if(m_live.HasNewVersion())
		{
			PostMessage(WM_NEWVERSION);
		}
	}
}

void __stdcall CWithCtrlDlg::NavigateComplete2Explorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	// TODO: 在此处添加消息处理程序代码
}


void __stdcall CWithCtrlDlg::BeforeNavigate2Explorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	// TODO: 在此处添加消息处理程序代码
	int nLen = _tcslen(REDIR_PREFIX);
	if(StrCmpNI(REDIR_PREFIX,_bstr_t(URL->bstrVal),nLen)==0)
	{
		*Cancel = VARIANT_TRUE;
		createGame();
		HttpWinInet down;
		_bstr_t redirURL(URL->bstrVal+ nLen);
		int ret = down.downFile(redirURL);
		if(ret)
		{
			std::string reDir;
			down.getFileString(reDir);
			enterGame(_bstr_t(reDir.c_str()));
			hideMainCtrl();
		}
	}
}


void CWithCtrlDlg::layoutSubItem()
{
	//布置窗口
	CAxWindow   axWeb   =   GetDlgItem(IDC_EXPLORER1); 
	RECT rcWeb = {24,116,690,480};
	axWeb.MoveWindow(&rcWeb);

	RECT rcBtn;
	m_btnMainPage.SubclassWindow(GetDlgItem(IDC_BTN_OFFPAGE));
	m_btnMainPage.LoadStateBitmaps(IDB_OFFPAGE_NORMAL, IDB_OFFPAGE_FOCUS, IDB_OFFPAGE_NORMAL);
	m_btnMainPage.GetWindowRect(&rcBtn);
	rcBtn.right = 97 + rcBtn.left;
	rcBtn.bottom =  37 + rcBtn.top;
	ScreenToClient(&rcBtn);
	m_btnMainPage.MoveWindow(&rcBtn);

	m_btnCharge.SubclassWindow(GetDlgItem(IDC_BTN_CHARGE));
	m_btnCharge.LoadStateBitmaps(IDB_CHARGE_NORMAL, IDB_CHARGE_FOCUS, IDB_CHARGE_NORMAL);
	m_btnCharge.GetWindowRect(&rcBtn);
	rcBtn.right = 97 + rcBtn.left;
	rcBtn.bottom =  37 + rcBtn.top;
	ScreenToClient(&rcBtn);
	m_btnCharge.MoveWindow(&rcBtn);


	m_btnBBS.SubclassWindow(GetDlgItem(IDC_BTN_BBS));
	m_btnBBS.LoadStateBitmaps(IDB_BBS_NORMAL, IDB_BBS_FOCUS, IDB_BBS_NORMAL);
	m_btnBBS.GetWindowRect(&rcBtn);
	rcBtn.right = 97 + rcBtn.left;
	rcBtn.bottom =  37 + rcBtn.top;
	ScreenToClient(&rcBtn);
	m_btnBBS.MoveWindow(&rcBtn);


	m_btnKeFu.SubclassWindow(GetDlgItem(IDC_BTN_CUS_SERVICE));
	m_btnKeFu.LoadStateBitmaps(IDB_KEFU_NORMAL, IDB_KEFU_FOCUS, IDB_KEFU_NORMAL);
	m_btnKeFu.GetWindowRect(&rcBtn);
	rcBtn.right = 97 + rcBtn.left;
	rcBtn.bottom =  37 + rcBtn.top;
	ScreenToClient(&rcBtn);
	m_btnKeFu.MoveWindow(&rcBtn);

	m_btnMinisize.SubclassWindow(GetDlgItem(IDC_BTN_MINBOX));
	m_btnMinisize.LoadStateBitmaps(IDB_MINBOX_NORMAL, IDB_MINBOX_FOCUS, IDB_MINBOX_NORMAL);
	m_btnMinisize.GetWindowRect(&rcBtn);
	rcBtn.right = 26 + rcBtn.left;
	rcBtn.bottom =  24 + rcBtn.top;
	ScreenToClient(&rcBtn);
	m_btnMinisize.MoveWindow(&rcBtn);

	m_btnClose.SubclassWindow(GetDlgItem(IDC_BTN_CLOSE));
	m_btnClose.LoadStateBitmaps(IDB_CLOSE_NORMAL, IDB_CLOSE_FOCUS, IDB_CLOSE_NORMAL);
	m_btnClose.GetWindowRect(&rcBtn);
	rcBtn.right = 30 + rcBtn.left;
	rcBtn.bottom =  30 + rcBtn.top;
	ScreenToClient(&rcBtn);
	m_btnClose.MoveWindow(&rcBtn);
}

LRESULT CWithCtrlDlg::OnBnClickedBtnMinbox(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	m_backDlg.ShowWindow(SW_MINIMIZE);
	return 0;
}


LRESULT CWithCtrlDlg::OnBnClickedBtnBbs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	stringt bbs;
	m_live.GetBBSURL(bbs);
	OpenURL(bbs);
	return 0;
}

LRESULT CWithCtrlDlg::OnBnClickedBtnOffpage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	stringt offPage;
	m_live.GetOffPageURL(offPage);
	OpenURL(offPage);
	return 0;
}

LRESULT CWithCtrlDlg::OnBnClickedBtnCharge(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	stringt charge;
	m_live.GetChargeURL(charge);
	OpenURL(charge);
	return 0;
}

LRESULT CWithCtrlDlg::OnBnClickedBtnCusService(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	stringt kefu;
	m_live.GetKefuURL(kefu);
	OpenURL(kefu);
	return 0;
}

void CWithCtrlDlg::createGame()
{
	if(!m_FlashDlg)
	{
		m_FlashDlg.Create(NULL,NULL);
	}
}

void CWithCtrlDlg::enterGame(BSTR url)
{
	RECT rcArea;
	::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
	m_FlashDlg.MoveWindow(&rcArea);
	m_FlashDlg.loadSwf(url);
}

void CWithCtrlDlg::showMainCtrl()
{
	m_backDlg.ShowWindow(SW_SHOW);
	ShowWindow(SW_SHOW);
	m_FlashDlg.DestroyWindow();
	m_bPlaying = FALSE;

	CComPtr <IWebBrowser>   spWeb; 
	GetIEBrowser(&spWeb);
	if(spWeb)
	{
		spWeb->Refresh();
	}
}

void CWithCtrlDlg::hideMainCtrl()
{
	m_backDlg.ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);
	if(m_FlashDlg.GetStyle() & WS_MAXIMIZE)
	{
		m_FlashDlg.ShowWindow(SW_SHOW);
	}
	else
	{
		m_FlashDlg.ShowWindow(SW_MAXIMIZE);
	}
	m_FlashDlg.SetActiveWindow();
	m_bPlaying = TRUE;
}

void CWithCtrlDlg::Exit()
{
	m_backDlg.CloseDialog(0);
	m_FlashDlg.DestroyWindow();
}

LRESULT CWithCtrlDlg::OnNavigatePage( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	stringt curPage;
 	if(m_live.GetCurrentPageURL(curPage))
	{
		CComPtr <IWebBrowser>   spWeb; 
		GetIEBrowser(&spWeb);
		if(spWeb)
		{
			spWeb->Navigate(_bstr_t(curPage.c_str()), NULL,   NULL,   NULL,   NULL);
		}
	}
	return 0;
}

LRESULT CWithCtrlDlg::OnGetURL( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
 	m_live.update();
	PostMessage(WM_NAVIGATEPAGE);
	return 0;
}

LRESULT CWithCtrlDlg::OnNewVersion( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	if(m_live.HasNewVersion())
	{
		if(MessageBox(_T("发现新版本，是否下载新版本?"),_T("你是否愿意下载新版本?"),MB_YESNO)==IDYES)
		{
			stringt newExe;
			if(m_live.getVersionExe(newExe))
				OpenURL(newExe);
		}
	}
	return 0;
}

void CWithCtrlDlg::OpenURL( stringt & strURL )
{
	stringt cmd = _T("IEXPLORE.exe");

	TCHAR szPath[MAX_PATH];
	if(SUCCEEDED(SHGetFolderPath(NULL,CSIDL_PROGRAM_FILES,NULL,0,szPath)))
	{
		int n = _tcslen(szPath);
		if(szPath[n-1]!='\\')
			StrCat(szPath,_T("\\Internet Explorer\\iexplore.exe"));
		else
			StrCat(szPath,_T("Internet Explorer\\iexplore.exe"));

		ShellExecute(m_hWnd,_T("open"),szPath,strURL.c_str(),NULL,SW_SHOWMAXIMIZED);
	}
	else
	{
		HINSTANCE h = ShellExecute(m_hWnd,_T("open"),	cmd.c_str(),strURL.c_str(),NULL,SW_SHOWMAXIMIZED);
		if((int)h<=32)
		{
			ShellExecute(m_hWnd,_T("open"),strURL.c_str(),NULL,NULL,SW_SHOWMAXIMIZED);
		}
	}
}
