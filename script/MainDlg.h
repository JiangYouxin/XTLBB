// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "script.h"
#include "AIScriptImpl.h"
#include "InjectScriptImpl.h"

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_HOTKEY, OnHotKey)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_CHECK1, BN_CLICKED, OnBnClickedCheck)
		COMMAND_HANDLER(IDC_CHECK2, BN_CLICKED, OnBnClickedCheck)
		COMMAND_HANDLER(IDC_CHECK3, BN_CLICKED, OnBnClickedCheck)
		COMMAND_HANDLER(IDC_CHECK4, BN_CLICKED, OnBnClickedCheck)
		COMMAND_HANDLER(IDC_EDIT1, EN_CHANGE, OnBnClickedCheck)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if(wParam == 822)
			ScriptFactory::OnTimer();
		return TRUE;
	}

	LRESULT OnHotKey(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		ScriptFactory::OnHotKey(wParam);
		return TRUE;
	}	

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		g_hWnd = m_hWnd;

		// center the dialog on the screen
		CenterWindow();

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

		// 读取配置，初始化全局变量以及各控件的状态
		CRegKey key;
		key.Create(HKEY_CURRENT_USER, "Software\\Yammaz\\tlbb_script");
		if(key.QueryDWORDValue("bServer", g_bServer) != ERROR_SUCCESS)
			g_bServer = 0;
		if(key.QueryDWORDValue("bSaveBmp", g_bSaveBmp) != ERROR_SUCCESS)
			g_bSaveBmp = 1;
		if(key.QueryDWORDValue("bWhite", g_bWhite) != ERROR_SUCCESS)
			g_bWhite = 0;
		if(key.QueryDWORDValue("bGamer", g_bGamer) != ERROR_SUCCESS)
			g_bGamer = 0;
		DWORD size = 256; 
		if(key.QueryStringValue("sName", g_sName, &size) != ERROR_SUCCESS)
			size = 0;
		g_sName[size] = '\0';
		key.Close();

		CButton(GetDlgItem(IDC_CHECK1)).SetCheck(g_bServer);
		CButton(GetDlgItem(IDC_CHECK2)).SetCheck(g_bSaveBmp);
		CButton(GetDlgItem(IDC_CHECK3)).SetCheck(g_bWhite);
		CButton(GetDlgItem(IDC_CHECK4)).SetCheck(g_bGamer);
		SetDlgItemText(IDC_EDIT1, g_sName);
		
		::EnableWindow(GetDlgItem(IDOK), FALSE);

		// 初始化各个Script
		ScriptFactory::Init(m_hWnd);
		ScriptFactory::CreateScript(new AIScriptImpl, 0, IDC_STATIC0);
		ScriptFactory::CreateScript(new AIScriptImpl, 1, IDC_STATIC1);
		ScriptFactory::CreateScript(new AIScriptImpl, 2, IDC_STATIC2);
		ScriptFactory::CreateScript(new AIScriptImpl, 3, IDC_STATIC3);
		ScriptFactory::CreateScript(new InjectScriptImpl, 4, IDC_STATIC4);
		ScriptFactory::CreateScript(new InjectScriptImpl, 5, IDC_STATIC5);	

		// 启动Timer
		SetTimer(822, 10);

		return TRUE;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// 更新全局变量
		g_bServer = CButton(GetDlgItem(IDC_CHECK1)).GetCheck();
		g_bSaveBmp = CButton(GetDlgItem(IDC_CHECK2)).GetCheck();
		g_bWhite = CButton(GetDlgItem(IDC_CHECK3)).GetCheck();
		g_bGamer = CButton(GetDlgItem(IDC_CHECK4)).GetCheck();
		GetDlgItemText(IDC_EDIT1, g_sName, 256);

		// 保存到注册表
		CRegKey key;
		key.Create(HKEY_CURRENT_USER, "Software\\Yammaz\\tlbb_script");
		key.SetDWORDValue("bServer", g_bServer);
		key.SetDWORDValue("bSaveBmp", g_bSaveBmp);
		key.SetDWORDValue("bWhite", g_bWhite);
		key.SetDWORDValue("bGamer", g_bGamer);
		key.SetStringValue("sName", g_sName);
		key.Close();

		// 灰掉Apply按钮
		::EnableWindow(GetDlgItem(IDOK), FALSE);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}

	LRESULT OnBnClickedCheck(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		::EnableWindow(GetDlgItem(IDOK), TRUE);
		return TRUE;
	}
};
