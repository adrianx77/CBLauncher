// FlashDlg.h : interface of the CFlashDlg  class
//
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"
#include "CaptionButton.h"
#include "TrayIconImpl.h"
#pragma once
class CWithCtrlDlg;


class CFlashDlg : public CAxDialogImpl<CFlashDlg >,
	public CCaptionButton<CFlashDlg>,
	public CTrayIconImpl<CFlashDlg>,
	public IDispEventImpl<IDC_EXPLORER1,CFlashDlg >,
	public IDispEventImpl<IDC_SHOCKWAVEFLASH1,CFlashDlg>
{
public:
	CWithCtrlDlg & m_CtrlDlg;
	CBitmap m_SettingBmp;
	CBitmap m_MuteBmp;
	CBitmap m_SoundBmp;
	CBitmap m_TrayBmp;
	TCHAR m_szTitlePre[100];
	enum { IDD = IDD_PLAY};
	enum { 
		ID_MUTE	= WM_USER + 101,
		ID_HOTKEYCODE = WM_USER + 102
	};

	BEGIN_MSG_MAP(CFlashDlg)
		CHAIN_MSG_MAP(CTrayIconImpl<CFlashDlg>)
		MESSAGE_HANDLER(WM_MOVE,OnMove)
		MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
		MESSAGE_HANDLER(WM_SIZE,OnSize)
		MESSAGE_HANDLER(WM_CLOSE,OnClose)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_HOTKEY,OnHotKey)
		COMMAND_ID_HANDLER(ID_FRESHGAME,OnFreshGame)
		COMMAND_ID_HANDLER(ID_HIDETOGGLE,OnHideToggle)
		COMMAND_ID_HANDLER(ID_CLEANCACHE,OnCleanCache)
		COMMAND_ID_HANDLER(ID_REHINT,OnReHint)
		COMMAND_ID_HANDLER(ID_FLASHSETTING,OnFlashSetting)
		COMMAND_ID_HANDLER(ID_EXIT,OnExit)
		COMMAND_ID_HANDLER(ID_MUTE,OnMute)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
	BEGIN_SINK_MAP(CFlashDlg)
		SINK_ENTRY(IDC_SHOCKWAVEFLASH1, 197, FlashCallShockwaveflash1)
	END_SINK_MAP()
	CFlashDlg(CWithCtrlDlg&dlg);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	HRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	HRESULT OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTest(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled);
	LRESULT OnFreshGame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled);
	LRESULT OnHideToggle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled);
	LRESULT OnCleanCache(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled);
	LRESULT OnReHint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled);
	LRESULT OnMute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled);
	LRESULT OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled);
	LRESULT OnFlashSetting(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled);
	HRESULT GetFlashCtrl(ShockwaveFlashObjects::IShockwaveFlash ** ppFlash);
	HRESULT loadSwf(BSTR URL);
protected:
	_bstr_t m_url;
	MSXML2::IXMLDOMDocumentPtr m_spXmlDoc;
	_variant_t CallFlashFun(BSTR request);
	_variant_t parseReturnValue(BSTR retValue);
	bool parseArgs(BSTR Method,stringt & methodName,std::vector<variant_t> & args);
public:
	POINT GetButtonPos(int index);
	
	bool setSoundMuteToggle();
	void cleanCache();
	void showFlashSetting();
	_bstr_t getLeftTask();
	void hideToggle();
	void noscroll();
	
	void __stdcall FlashCallShockwaveflash1(BSTR request);
};