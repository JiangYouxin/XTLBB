// InjectDLL.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "detours/detours.h"
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdarg.h>

#include "TlbbStruct.h"

typedef DWORD SOCKET;
extern "C" int WINAPI send(SOCKET s, const char *buf, int len, int flags);
typedef int (WINAPI *pSendProc)(SOCKET s, const char *buf, int len, int flags);

pSendProc pOrigSend;
HWND g_hWnd;

// 存储上一次的选怪操作
SELECT g_select;

// g_select是否已经初始化
bool g_select_init = false;

DWORD g_mode = 0;

#define WM_CHANGE_MODE (WM_USER + 0xc531)

void MyTrace(const char *format, ...)
{
	va_list v;
	va_start(v, format);

	char buf[1024];
	_vsnprintf_s(buf, 1024, format, v);

	OutputDebugStringA(buf);
}

void DumpSendBuffer(const char *buf, int len)
{
	char buffer[256000];
	for(int i = 0; i < len; i++)
	{
		wsprintfA(buffer + i * 3, "%02x ", (unsigned char)buf[i]);
	}
	OutputDebugString(buffer);
}

int send_filter(char * dst, const char * src, int len)
{
	unsigned char command = (unsigned char)src[0];

	// 普通打怪的辅助
	if (g_mode == 0)
	{
		// 保存最新的选怪包
		if(command == SELECT_CODE)
		{
			memcpy(&g_select, src, sizeof(SELECT));
			g_select_init = true;
		}

		// 如果是取消攻击包，用选怪包代替
		if(command == UN_SKILL_CODE && g_select_init)
		{
			PSELECT pse = (PSELECT)src;
			PSELECT ps = (PSELECT)dst;
			memcpy(dst, &g_select, sizeof(SELECT));
			ps->syn = pse->syn;
			return sizeof(SELECT);
		}
	}
	//	强行重连的处理
	else if (g_mode == 1)
	{
		PSELECT pse = (PSELECT)src;
		PSELECT ps = (PSELECT)dst;
		memset(dst, 0xC4, sizeof(SELECT));

		ps->syn = pse->syn;

		g_mode = 0;

		return sizeof(SELECT);
	}

	// 正常情况什么都不做
	memcpy(dst, src, len);
	// DumpSendBuffer(src, len);

	return len; 
}


int WINAPI fake_send(SOCKET s, const char * buf, int len, int flags)
{ 
	char buffer[4096];
	const char * lastBuffer = buf;
	int length = 0;

	while(lastBuffer - buf < len)
	{
		int l = *(PSHORT)(lastBuffer + 2) + 6;

		if (l < 0 || lastBuffer + l - buf > len)
		{
			MyTrace("sb in fake_send.");
			return pOrigSend(s, buf, len, flags);
		}
		length += send_filter(buffer + length, lastBuffer, l);
		lastBuffer += l;
	}

	int n = pOrigSend(s, buffer, length, flags);

	if(n == length) 
		return len;
	else if(n == -1)
		return -1;
	else
		return 0;
}

void InitHook()
{
	pOrigSend = send;
	g_hWnd = GetForegroundWindow();

	MyTrace("I am in.");

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((PVOID *)&pOrigSend, fake_send);
	DetourTransactionCommit();	
}

void FiniHook()
{
	OutputDebugString("I am out.");

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach((PVOID *)&pOrigSend, fake_send);
	DetourTransactionCommit();		
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InitHook();
		break;
	case DLL_PROCESS_DETACH:
		FiniHook();
		break;
	}
    return TRUE;
}

