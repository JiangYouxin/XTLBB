#include "stdafx.h"
#include "InjectScriptImpl.h"

#include "InjectHelper.h"

InjectScriptImpl::InjectScriptImpl() 
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
void InjectScriptImpl::DoStart()
{
	StartInject(m_hWnd, szDllName);
}
void InjectScriptImpl::DoStop()
{
	StopInject(m_hWnd, szDllName);
}
char * InjectScriptImpl::GetName()
{
	if(!m_start)
		return "侵入式打怪";
	else 
		return "侵入式打怪(开启)";
}