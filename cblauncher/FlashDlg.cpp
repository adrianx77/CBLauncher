#include "stdafx.h"
#include "FlashDlg.h"
#include "WithCtrlDlg.h"

CFlashDlg::CFlashDlg(CWithCtrlDlg&dlg):m_CtrlDlg(dlg)
{
	m_SettingBmp.LoadBitmap(IDB_SETTING);
	m_MuteBmp.LoadBitmap(IDB_MUTE);
	m_SoundBmp.LoadBitmap(IDB_SOUND);
	m_TrayBmp.LoadBitmap(IDB_TRAYICON);
	SetDefaultItem(ID_HIDETOGGLE);
	StrCpy(m_szTitlePre,TITLE_NAME);
}

LRESULT CFlashDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);
	RegisterHotKey(m_hWnd,ID_HOTKEYCODE,0,VK_F10);
	InstallIcon(L"游戏客户端",hIconSmall,IDR_MENU_TRAYICON);
	CenterWindow();
	InsertButton(ID_HIDETOGGLE,TRUE,m_TrayBmp);
	InsertButton(ID_MUTE,TRUE,m_SoundBmp);
	InsertButton(ID_FLASHSETTING,TRUE,m_SettingBmp);
	AtlAdviseSinkMap (this,true);
	return TRUE;
}

HRESULT CFlashDlg::loadSwf( BSTR URL )
{
	CComPtr<IWebBrowser> spWeb;
	m_url = URL;
	ShockwaveFlashObjects::IShockwaveFlashPtr spFlash;
	GetFlashCtrl(&spFlash);
	if(spFlash)
	{
		spFlash->LoadMovie(0,m_url);
	}
	return E_FAIL;
}

LRESULT CFlashDlg::OnMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = true;
	return 0;
}

LRESULT CFlashDlg::OnDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	AtlAdviseSinkMap ( this, false );
	UnregisterHotKey(m_hWnd,ID_HOTKEYCODE);
	CaptionClear();
	return 0;
}

LRESULT CFlashDlg::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	switch(wParam)
	{
	case SIZE_MAXHIDE:
	case SIZE_MAXIMIZED:
	case SIZE_MAXSHOW:
	case SIZE_MINIMIZED:
	case SIZE_RESTORED:
		break;
	}
	RECT rcClient;
	GetClientRect(&rcClient);
	CAxWindow   axWeb   =   GetDlgItem(IDC_SHOCKWAVEFLASH1); 
	axWeb.MoveWindow(&rcClient);
	return 0;
}

HRESULT CFlashDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//询问是否关闭
	m_CtrlDlg.Exit();
	return 0;
}

HRESULT CFlashDlg::GetFlashCtrl( ShockwaveFlashObjects::IShockwaveFlash ** ppFlash )
{
	CAxWindow   axWeb   =   GetDlgItem(IDC_SHOCKWAVEFLASH1); 
	return axWeb.QueryControl(ppFlash); 
}

void __stdcall CFlashDlg::FlashCallShockwaveflash1(BSTR request)
{
	// TODO: 在此处添加消息处理程序代码
	ShockwaveFlashObjects::IShockwaveFlashPtr spFlash;
	GetFlashCtrl(&spFlash);
	if(!spFlash)
		return ;

	std::vector<variant_t> args;
	stringt name;
	if(parseArgs(request,name,args))
	{
		if(0==StrCmpI(name.c_str(),L"geturl"))
		{
			stringt newURL= args[0].bstrVal;
			m_CtrlDlg.OpenURL(newURL);
		}
	}
	//
	spFlash->SetReturnValue(_bstr_t("<string/>"));
}


bool CFlashDlg::setSoundMuteToggle()
{
	BSTR request = L"<invoke name= \"setSoundMuteToggle\" returntype=\"xml\"></invoke>";
	try
	{
		_variant_t v = CallFlashFun(request);
		return v.boolVal != VARIANT_FALSE;
	}
	catch (_com_error&)
	{
		return true;
	}
}

void CFlashDlg::showFlashSetting()
{
	BSTR request = L"<invoke name= \"showFlashSettings\" returntype=\"xml\"></invoke>";
	try
	{
		CallFlashFun(request);
	}
	catch (_com_error&)
	{
	}
}


_bstr_t CFlashDlg::getLeftTask()
{
	BSTR request = L"<invoke name= \"getLeftTask\" returntype=\"xml\"></invoke>";
	try
	{
		_variant_t ret = CallFlashFun(request);
		return ret.bstrVal;
	}
	catch (_com_error&)
	{
		return L"";
	}
}

_variant_t CFlashDlg::CallFlashFun( BSTR request )
{
	ShockwaveFlashObjects::IShockwaveFlashPtr spFlash;
	GetFlashCtrl(&spFlash);
	if(!spFlash)
		return L"";
	_bstr_t ret =  spFlash->CallFunction(request);

	return parseReturnValue(ret);
}

void CFlashDlg::cleanCache()
{
	BSTR request = L"<invoke name= \"cleanCache\" returntype=\"xml\"></invoke>";
	try
	{
		CallFlashFun(request);
	}
	catch (_com_error&)
	{
	}
}

bool CFlashDlg::parseArgs(BSTR Method,stringt & methodName,std::vector<variant_t> & args)
{
	_variant_t vEmpty;
	_variant_t bValTrue(true);
	_variant_t bValFalse(false);
	//sure xmldoc
	if(!m_spXmlDoc)
	{
		HRESULT hr = m_spXmlDoc.CreateInstance(__uuidof(MSXML2::DOMDocument30));

		if(FAILED(hr))
			return false;

		m_spXmlDoc->async = VARIANT_FALSE;
	}

	if(!m_spXmlDoc->loadXML(Method))
		return false;

	MSXML2::IXMLDOMNodePtr spNameNode= m_spXmlDoc->selectSingleNode(_T("invoke//@name"));
	if(!spNameNode)
		return false;
	methodName = (BSTR)spNameNode->text;

	MSXML2::IXMLDOMNodePtr spInvoke= m_spXmlDoc->selectSingleNode(_T("invoke"));
	if(!spInvoke)
		return false;

	MSXML2::IXMLDOMNodePtr spArgments = spInvoke->firstChild;
	if(!spArgments)
		return false;
	
	MSXML2::IXMLDOMNodeListPtr spArgNodes = spArgments->childNodes;
	if(!spArgNodes)
		return false;

	LONG count;
	spArgNodes->get_length(&count);
	for(long i=0;i<count;i++)
	{
		MSXML2::IXMLDOMNodePtr spArg = spArgNodes->item[i];
		if(0==StrCmpI(spArg->nodeName,L"string"))
		{
			_variant_t v = (BSTR)spArg->text;
			args.push_back(v);
		}
		else if(0==StrCmpI(spArg->nodeName,L"null"))
		{
			args.push_back(vEmpty);
		}
		else if(0==StrCmpI(spArg->nodeName,L"true"))
		{
			args.push_back(bValTrue);
		}
		if(0==StrCmpI(spArg->nodeName,L"false"))
		{
			args.push_back(bValFalse);
		}
	}
	
	return true;
}
_variant_t CFlashDlg::parseReturnValue( BSTR retValue )
{
	_variant_t vEmpty;
	_variant_t bValTrue(true);
	_variant_t bValFalse(false);
	//sure xmldoc
	if(!m_spXmlDoc)
	{
		HRESULT hr = m_spXmlDoc.CreateInstance(__uuidof(MSXML2::DOMDocument30));
		
		if(FAILED(hr))
			return vEmpty;

		m_spXmlDoc->async = VARIANT_FALSE;
	}

	if(!m_spXmlDoc->loadXML(retValue))
		return vEmpty;
	MSXML2::IXMLDOMElementPtr elPtr = m_spXmlDoc->documentElement;
	_bstr_t n = elPtr->tagName;
	_bstr_t t = m_spXmlDoc->text;
	if(0==StrCmpI(n,L"string"))
	{
		return (BSTR)t;
	}
	if(0==StrCmpI(n,L"null"))
	{
		return vEmpty;
	}
	if(0==StrCmpI(n,L"true"))
	{

		return bValTrue;
	}
	if(0==StrCmpI(n,L"false"))
	{
		return bValFalse;
	}

	return 0;
}

LRESULT CFlashDlg::OnTest( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled )
{
	parseReturnValue(L"<null/>");
	return 0;
}

HRESULT CFlashDlg::OnHotKey( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	hideToggle();
	return TRUE;
}

LRESULT CFlashDlg::OnFreshGame( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled )
{
	m_CtrlDlg.showMainCtrl();
	return 0;
}

LRESULT CFlashDlg::OnHideToggle( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled )
{
	hideToggle();
	return 0;
}

LRESULT CFlashDlg::OnCleanCache( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled )
{
	cleanCache();
	return 0;
}

LRESULT CFlashDlg::OnReHint( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled )
{
	return 0;
}

LRESULT CFlashDlg::OnExit( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled )
{
	m_CtrlDlg.Exit();
	return 0;
}

LRESULT CFlashDlg::OnFlashSetting( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled )
{
	showFlashSetting();
	return 0;
}

void CFlashDlg::hideToggle()
{
	if(GetStyle() & WS_VISIBLE)
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		ShowWindow(SW_SHOW);
	}
}

LRESULT CFlashDlg::OnMute( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled )
{
	BOOL bMute = setSoundMuteToggle();
	if(!bMute)
	{
		ModifyButton(ID_MUTE,m_SoundBmp);
	}
	else
	{
		ModifyButton(ID_MUTE,m_MuteBmp);
	}

	return 0;
}


