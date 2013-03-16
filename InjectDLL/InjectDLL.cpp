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
extern "C" int WINAPI recv(SOCKET s, char *buf, int len, int flags);
typedef int (WINAPI *pSendProc)(SOCKET s, const char *buf, int len, int flags);
typedef int (WINAPI *pRecvProc)(SOCKET s, char *buf, int len, int flags);

pSendProc pOrigSend;
pRecvProc pOrigRecv;
HWND g_hWnd;
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
	// 存储上一次的打怪操作
	static SKILL g_skill;
	// g_skill是否已经初始化
	static bool g_skill_init = false;

	unsigned char command = (unsigned char)src[0];

	// 保存最新的打怪包
	if(!g_skill_init && command == SKILL_CODE)
	{
		memcpy(&g_skill, src, sizeof(SKILL));
		g_skill_init = true;
	}
	// 如果是选怪包，用打怪包代替
	if(command == SELECT_CODE && g_skill_init)
	{
		PSELECT pse = (PSELECT)src;
		g_skill.id = pse->id;
		g_skill.syn = pse->syn;
		memcpy(dst, &g_skill, sizeof(SKILL));
		return sizeof(SKILL);
	}
	// 正常情况什么都不做
	memcpy(dst, src, len);
	// DumpSendBuffer(src, len);

	return len; 
}

int WINAPI fake_recv(SOCKET s, char * buf, int len, int flags)
{
	return pOrigRecv(s, buf, len, flags);
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
	DetourAttach((PVOID *)&pOrigRecv, fake_recv);
	DetourTransactionCommit();	
}

void FiniHook()
{
	OutputDebugString("I am out.");

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach((PVOID *)&pOrigSend, fake_send);
	DetourDetach((PVOID *)&pOrigRecv, fake_recv);
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

