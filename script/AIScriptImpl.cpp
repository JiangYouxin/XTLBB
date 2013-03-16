#include "stdafx.h"
#include "AIScriptImpl.h"

#include "InjectHelper.h"
#include "FileUtility.h"

void AIScriptImpl::DoStart()
{
	StartInject(m_hWnd, GetInjectDllName());

	m_mode = 0;
	m_fast = false;
	m_fastCount = 0;
	m_objectCount = 0;
	m_n = 0;
	m_xxxCount = 0;

	/*
	HDC hdc = CreateCompatibleDC(m_hDC);
	HBITMAP hbmp = CreateCompatibleBitmap(m_hDC, 600, 300);
	SelectObject(hdc, hbmp);
	BitBlt(hdc, 0, 0, 600, 300, m_hDC, 0, 0, SRCCOPY);
	DeleteDC(hdc);
	
	OpenClipboard(g_hWnd);
	EmptyClipboard(); 
	SetClipboardData(CF_BITMAP, hbmp);
	CloseClipboard();

	DeleteObject(hbmp);
	*/
}
void AIScriptImpl::DoStop()
{
	StopInject(m_hWnd, GetInjectDllName());
}
void AIScriptImpl::DoReload()
{
	m_mode = (m_mode + 1) % 3;
	m_n = 0;
}
void AIScriptImpl::DoOnTimer()
{
	m_xxxCount++;

	if (m_xxxCount == 100)
		SendKeyPress(m_hWnd, 118);
	if (m_xxxCount == 200)
		SendKeyPress(m_hWnd, 119);
	if (m_xxxCount == 300)
		SendKeyPress(m_hWnd, 120);
	if (m_xxxCount == 400)
		m_xxxCount = 0;

	if(m_fast)
	{
		m_fastCount++;
		if(m_fastCount >= 3000)
			m_fast = false;
	}
	if(m_gamer)
	{
		m_gamerCount++;
		if(m_gamerCount >= 3000)
			m_gamer = false;			
	}
	if(m_timerCount <= 10) return;

	OBJECT_COLOR oc = GetObjectColor();

	if((oc != INVALID_OBJECT) && IsAlive())
	{
		DWORD limit;
		DWORD key;
		if(oc == WHITE_OBJECT)
		{
			goto another;
		}
		else if(oc == GREEN_OBJECT)
		{
			if(m_mode == 2) 
				limit = 1;
			else
				limit = 30;
			key = 123;
		}
		else 
		{
			key = 123;
			if(m_mode == 1)
				limit =	1;
			else if(m_fast)
				limit = 15;
			else
				limit = 350;
		}
		if(m_n < limit)
		{				
			if(m_n %2 == 0)
				SendKeyPress(m_hWnd, key);	
			m_n++;
			m_timerCount = 1;
			return;		
		}
	}
another:
	SendKeyPress(m_hWnd, 122);
	m_n = 0;
	m_timerCount = 1;
}

char * AIScriptImpl::GetName()
{
	if(!m_start)
		return "智能打怪";
	else if(m_mode == 0)
		return "智能打怪(自动)";
	else if(m_mode == 1)
		return "智能打怪(快速切怪)";
	else 
		return "智能打怪(帮打模式)";
}
