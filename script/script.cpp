// script.cpp : main source file for script.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

#include "MainDlg.h"
#include "UDPBroadcast.h"
#include "script.h"

TlbbScript * ScriptFactory::scripts[8];
DWORD ScriptFactory::ids[8];
HWND ScriptFactory::g_hwnd;
UDPBroadcast * TlbbScript::g_pbc;
IQMFunctionPtr TlbbScript::g_obj;

DWORD g_bServer;
DWORD g_bSaveBmp;
DWORD g_bWhite;
DWORD g_bGamer;
char g_sName[256];

CAppModule _Module;

HWND g_hWnd;

void MyMessageBox(char * m)
{
	MessageBox(0, m, m, 0);
}

int LoadDriver()
{
	SC_HANDLE sh = NULL;
	SC_HANDLE rh = NULL;
	__try
	{
		sh = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if(!sh)
		{
			MyMessageBox("打开SCManager失败。");
			__leave;
		}
		rh = CreateService(sh, "tlbb_script", "tlbb_script", SERVICE_ALL_ACCESS, 
			SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, 
			"C:\\tlbb_script.sys", NULL, NULL, NULL, NULL, NULL);
		if(!rh)
		{
			if(GetLastError() == ERROR_SERVICE_EXISTS)
			{
				rh = OpenService(sh, "TestDriver", SERVICE_ALL_ACCESS);
				if(!rh)
				{
					MyMessageBox("打开Service失败！");
					__leave;
				}
			}
			else
			{
				MyMessageBox("创建Service失败！");
				__leave;
			}
		}
		StartService(rh, 0, NULL);
	}
	__finally
	{
		if(rh) 
		{
			DeleteService(rh);
			CloseServiceHandle(rh);
		}
		if(sh) CloseServiceHandle(sh);
	}
	return 0;
}

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainDlg dlgMain;

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	// LoadDriver();
	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
