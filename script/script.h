// script.h
#pragma once


#include "UDPBroadcast.h"
#include "TlbbScript.h"

extern HWND g_hWnd;

#pragma comment(lib, "winmm.lib")

extern DWORD g_bServer;
extern DWORD g_bSaveBmp;
extern DWORD g_bWhite;
extern DWORD g_bGamer;
extern char g_sName[256];


class ScriptFactory
{
public:
	static void Init(HWND hwnd)
	{
		TlbbScript::InitScript();
		for(int i = 0; i < 8; i++)
			scripts[i] = NULL;
		g_hwnd = hwnd;
	}
	static void CreateScript(TlbbScript * psb, int hotKey, DWORD id)
	{
		scripts[hotKey] = psb;
		ids[hotKey] = id;
		RegisterHotKey(g_hwnd, hotKey + 100, MOD_ALT, hotKey + 116);
		RegisterHotKey(g_hwnd, hotKey + 200, MOD_CONTROL, hotKey + 116);

		char buffer[256];
		wsprintfA(buffer, "F%d: %s", hotKey + 5, psb->GetName());
		SetWindowText(GetDlgItem(g_hwnd, id), buffer);
	}
	static void OnTimer()
	{
		for(int i = 0; i < 8; i++) if(scripts[i])
			scripts[i]->OnTimer();
		
		char buffer[256];
		
		sockaddr_in addr;
		if(g_bServer && TlbbScript::g_pbc->RecvBroadcast(buffer, 256, &addr) > 0)
		{
			PlaySound("C:\\lang.wav", NULL, SND_ASYNC | SND_FILENAME);
			SetWindowText(g_hwnd, buffer);
		}
	}
	static void OnHotKey(int wParam)
	{
		if(wParam >= 200)
		{
			scripts[wParam - 200]->Start();
			
			char buffer[256];
			wsprintfA(buffer, "->-> F%d: %s", wParam - 200 + 5, scripts[wParam - 200]->GetName());
			SetWindowText(GetDlgItem(g_hwnd, ids[wParam - 200]), buffer);
		}
		else
		{
			scripts[wParam - 100]->Stop();

			char buffer[256];
			wsprintfA(buffer, "F%d: %s", wParam - 100 + 5, scripts[wParam - 100]->GetName());
			SetWindowText(GetDlgItem(g_hwnd, ids[wParam - 100]), buffer);
		}
	}
public:
	static TlbbScript * scripts[8];
	static DWORD ids[8];
	static HWND g_hwnd;
};