#include "stdafx.h"
#include "FileUtility.h"

static char szDllName[MAX_PATH + 1] = { 0 };
char * GetInjectDllName()
{
	if (!szDllName[0]) 
	{
		int i;
		GetModuleFileName(NULL, szDllName, MAX_PATH);
		for (i = strlen(szDllName) - 1; i >= 0; i--)
		{
			if (szDllName[i] == '\\')
				break;
		}
		strcpy_s(szDllName + i + 1, MAX_PATH - i - 1, "InjectDll.dll");
	}
	return szDllName;
}