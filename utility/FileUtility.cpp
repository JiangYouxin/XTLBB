#include "FileUtility.h"

static void CheckInitPathName(char *lpszPathName, const char *lpszFileName, HMODULE hModule)
{
	if (!lpszPathName[0]) 
	{
		int i;
		GetModuleFileNameA(hModule, lpszPathName, MAX_PATH);
		for (i = strlen(lpszPathName) - 1; i >= 0; i--)
		{
			if (lpszPathName[i] == '\\')
				break;
		}
		strcpy_s(lpszPathName + i + 1, MAX_PATH - i - 1, lpszFileName);
	}
}

static char szDllName[MAX_PATH + 1] = { 0 };
char * GetInjectDllName()
{
	CheckInitPathName(szDllName, "InjectDll.dll", NULL);
	return szDllName;
}

static char szLuaName[MAX_PATH + 1] = { 0 };
char * GetLuaFileName(HMODULE hModule)
{
	CheckInitPathName(szLuaName, "script.lua", hModule);
	return szLuaName;
}