#include "stdafx.h"
#include "InjectHelper.h"

#include <tlhelp32.h>

void StartInject(HWND hWnd, LPCSTR lpszDllName)
{
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	char * pFileName = NULL;
	__try
	{
		DWORD pid = 0;
		GetWindowThreadProcessId(hWnd, &pid);

		hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE,
			FALSE, pid);
		if(!hProcess)
		{
			__leave;
		}

		int cb = strlen(lpszDllName) + 1;

		pFileName = (char *)VirtualAllocEx(hProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
		if(!pFileName)
		{
			__leave;
		}

		if(!WriteProcessMemory(hProcess, pFileName, lpszDllName, cb, NULL))
		{
			__leave;
		}

		PTHREAD_START_ROUTINE pfn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), 
			"LoadLibraryA");
		if(!pfn)
		{
			__leave;
		}

		hThread = CreateRemoteThread(hProcess, NULL, 0, pfn, pFileName, 0, NULL);
		if(!hThread)
		{
			__leave;
		}

		WaitForSingleObject(hThread, INFINITE);
	}
	__finally
	{
		if(pFileName)
			VirtualFreeEx(hProcess, pFileName, 0, MEM_RELEASE);
		if(hThread) CloseHandle(hThread);
		if(hProcess) CloseHandle(hProcess);
	}
}

void StopInject(HWND hWnd, LPCSTR lpszDllName)
{
	HANDLE hth = NULL;
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;

	__try
	{
		DWORD pid = 0;
		GetWindowThreadProcessId(hWnd, &pid);

		hth = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
		if(!hth)
		{
			__leave;
		}
		
		MODULEENTRY32 me = {sizeof(me)};
		BOOL ff = FALSE;
		for(BOOL fm = Module32First(hth, &me); fm; fm = Module32Next(hth, &me))
		{
			ff = (lstrcmpi(me.szExePath, lpszDllName) == 0 || lstrcmpi(me.szModule, lpszDllName) == 0);
			if(ff) break;
		}
		if(!ff)
		{
			__leave;
		}

		hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE,
			FALSE, pid);
		if(!hProcess)
		{
			__leave;
		}

		PTHREAD_START_ROUTINE pfn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), 
			"FreeLibrary");
		if(!pfn)
		{
			__leave;
		}

		hThread = CreateRemoteThread(hProcess, NULL, 0, pfn, me.modBaseAddr, 0, NULL);
		if(!hThread)
		{
			__leave;
		}

		WaitForSingleObject(hThread, INFINITE);
	}
	__finally
	{
		if(hth) CloseHandle(hth);
		if(hThread) CloseHandle(hThread);
		if(hProcess) CloseHandle(hProcess);
	}
}