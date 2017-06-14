#include "stdafx.h"
#include "BackGroundDlg.h"
CBackGroundDlg::CBackGroundDlg():m_WithCtrlDlg(*this)
{
	m_bCanBeHit = FALSE;
}



BOOL CBackGroundDlg::CanBeHit()
{
	return m_bCanBeHit;
}
LRESULT CBackGroundDlg::OnNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bCanBeHit = TRUE;
	bHandled = true;
	return HTCAPTION;
}

LRESULT CBackGroundDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	m_upLayered.attach(m_hWnd);
	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();

	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);


	m_WithCtrlDlg.Create(m_hWnd,NULL);
	m_WithCtrlDlg.ShowWindow(SW_SHOW);
	ModifyStyle(WS_CAPTION,0,0);

	if(m_upLayered.loadBackground(IDR_PNG_BK,_Module.m_hInstResource))
	{
		SIZE s = m_upLayered.getBkImageSize();
		MoveWindow(0,0,s.cx,s.cy);
		CenterWindow();
		RECT psR;
		GetWindowRect(&psR);
		m_WithCtrlDlg.MoveWindow(psR.left,psR.top,s.cx,s.cy);
		m_upLayered.UpdateLayer();
	}

	TCHAR szExe[MAX_PATH];
	GetModuleFileName(_Module.m_hInst,szExe,MAX_PATH);
	createShortcutDesktop(szExe);

	//SetTimer(1,200,NULL);
	return TRUE;
}

LRESULT CBackGroundDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CBackGroundDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}
LRESULT CBackGroundDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam==1)
	{
		if(NULL !=HWND(m_WithCtrlDlg))
		{
			SetWindowPos(m_WithCtrlDlg,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE );
		}
		bHandled = true;
	}
	return 0;
}

LRESULT CBackGroundDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(::IsWindow(m_hWnd))
	{
		if(CanBeHit())
		{
			RECT rcBackDlg,rcWithCtrl;
			GetWindowRect(&rcBackDlg);
			m_WithCtrlDlg.GetWindowRect(&rcWithCtrl);
			m_WithCtrlDlg.MoveWindow(rcBackDlg.left,rcBackDlg.top,rcWithCtrl.right-rcWithCtrl.left,rcWithCtrl.bottom-rcWithCtrl.top,TRUE);
		}
	}
	bHandled = true;

	return 0;
}

void CBackGroundDlg::CancelBeHit()
{
	m_bCanBeHit = FALSE;
}

LRESULT CBackGroundDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

void CBackGroundDlg::CloseDialog( int nVal )
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

void CBackGroundDlg::createShortcutDesktop( LPCTSTR pszExe )
{
	bool bRet = false;
	IShellLink* psl;
	LPITEMIDLIST pidl;
	LPMALLOC pShellMalloc;
	stringt strDesktopPath;

	if (SUCCEEDED(SHGetMalloc(&pShellMalloc))) 
	{
		TCHAR Path[MAX_PATH + 1];
		if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &pidl))) 
		{
			if (SHGetPathFromIDList(pidl, Path)) 
			{
				strDesktopPath = Path;
			}
			pShellMalloc->Free(pidl);
			
		}
		pShellMalloc->Release();
	}
	
	if(*strDesktopPath.rbegin()!='\\')
	{
		strDesktopPath += _T("\\");
	}
	
	TCHAR szFileTitle[MAX_PATH] = TITLE_NAME;
	
	TCHAR szExePath[MAX_PATH] = {0};
	StrCpy(szExePath,pszExe);
	LPTSTR pFileName = PathFindFileName(szExePath);
	if(pFileName!=NULL)
		*pFileName = '\0';

	stringt f = szExePath;
	stringt d = strDesktopPath;

	if(StrCmpI(f.c_str(),d.c_str())==0)
	{
		return;
	}

	stringt str;
	str = strDesktopPath;
	str += szFileTitle;
	str += _T(".lnk");

	DWORD dwAtt = GetFileAttributes(str.c_str());
	if(dwAtt!=-1)
	{
		return;
	}

	HRESULT hr = CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(LPVOID*)&psl);
	if (SUCCEEDED(hr)) 
	{
		IPersistFile* ppf;
		psl->SetPath(pszExe);
		psl->SetDescription(TITLE_NAME);
		psl->SetShowCmd(SW_SHOW);
		if (SUCCEEDED(psl->QueryInterface(IID_IPersistFile, (LPVOID *)&ppf))) 
		{
			_bstr_t mbw = str.c_str();
			if (SUCCEEDED(ppf->Save(mbw, TRUE)))
				bRet = true;
			ppf->Release();
		}
		psl->Release();
	}	
}
