#include "stdafx.h"
#include "TlbbInstance.h"

void TlbbInstance::SendKeyPress(HWND hWnd, int code)
{
	PostMessage(hWnd, WM_KEYDOWN, code, MapVirtualKey(code, 0) >> 8 | 0x01);
	PostMessage(hWnd, WM_KEYUP, code, 0xC0000000 | MapVirtualKey(code, 0) >> 8 | 0x01);
}

COLORREF TlbbInstance::GetColor(HWND hWnd, HDC hDC, int x, int y)
{
	RECT r;
	GetClientRect(hWnd, &r);

	COLORREF cr = GetPixel(hDC, x + r.left, y + r.top);

	char szBuf[256];
	wsprintfA(szBuf, "%d %d %x\n", x, y, cr);
	OutputDebugString(szBuf);

	return cr;
}

BOOL TlbbInstance::IsFullLife()
{
	COLORREF cr = GetColor(m_hWnd, m_hDC, 400, 32);
	return IsRed(cr);
}
BOOL TlbbInstance::IsSelfFullLife()
{
	COLORREF cr = GetColor(m_hWnd, m_hDC, 196, 37);
	return IsRed(cr);
}
BOOL TlbbInstance::IsGamerFullLife(int id)
{
	int y = 129 + 45 * id;

	COLORREF cr = GetColor(m_hWnd, m_hDC, 44, y);
	if(!IsBlack(cr)) return TRUE;

	cr = GetColor(m_hWnd, m_hDC, 45, y);
	if(!IsRedRed(cr)) return TRUE;

	cr = GetColor(m_hWnd, m_hDC, 140, y);
	if(!IsBlack(cr)) return TRUE;

	int c = 0;
	
	// return FALSE;

	for(int x = 46; x < 140; x++)
	{
		cr = GetColor(m_hWnd, m_hDC, x, y);
		if(!IsBlack(cr) && !IsRedRed(cr)) 
			c++;
	}

	return c > 10;
}

// 怪是否活着
BOOL TlbbInstance::IsAlive()
{
	COLORREF cr = GetColor(m_hWnd, m_hDC, 274, 33);
	return IsRed(cr);
}
// 获取目标的颜色（绿色表示未动过，在鹿鼎记是白色；红色表示己方；白色表示对方，在鹿鼎记是灰色）
TlbbInstance::OBJECT_COLOR TlbbInstance::GetObjectColor()
{
	COLORREF cr = GetColor(m_hWnd, m_hDC, 276, 26);

	if(IsRed(cr))
		return RED_OBJECT;
	else if(IsGreen(cr))
		return GREEN_OBJECT;
	else if(IsWhite(cr))
		return WHITE_OBJECT;
	else
		return INVALID_OBJECT;			
}
// 目标是否是红色
BOOL TlbbInstance::IsRed(COLORREF cr)
{
	return GetRValue(cr) >= 180 && GetGValue(cr) < 130 && GetBValue(cr) < 130;
}
// 目标是否是绿色（鹿鼎记里面是白色）
BOOL TlbbInstance::IsGreen(COLORREF cr)
{
	return GetRValue(cr) >= 150 && GetGValue(cr) >= 150 && GetBValue(cr) >= 150;
}
// 目标是否是白色（鹿鼎记里面是灰色）
BOOL TlbbInstance::IsWhite(COLORREF cr)
{
	return GetRValue(cr) > 90 && GetGValue(cr) > 90 && GetBValue(cr) > 90
		&& GetRValue(cr) < 130 && GetGValue(cr) < 130 && GetBValue(cr) < 130;
}

BOOL TlbbInstance::IsGamer()
{
	COLORREF cr = GetColor(m_hWnd, m_hDC, 279, 36);
	if(IsBlue(cr)) 
		return TRUE;
	cr = GetColor(m_hWnd, m_hDC, 279, 42);
	if(IsYellow(cr)) 
		return TRUE;
	return FALSE;	
}

BOOL TlbbInstance::IsBoss()
{
	COLORREF cr = GetColor(m_hWnd, m_hDC, 279, 36);
	if(IsRed(cr)) 
		return TRUE;
	cr = GetColor(m_hWnd, m_hDC, 279, 42);
	if(IsRed(cr)) 
		return TRUE;
	return FALSE;	
}

void TlbbInstance::LeftClick(int x, int y)
{
	POINT p;
	p.x = x;
	p.y = y;
	ClientToScreen(m_hWnd, &p);
	SetCursorPos(p.x, p.y);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	// mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);		
}

BOOL TlbbInstance::IsRedRed(COLORREF cr)
{
	return GetRValue(cr) >= 0xF0 && GetGValue(cr) < 0x20 && GetBValue(cr) < 0x20;
}
BOOL TlbbInstance::IsBlack(COLORREF cr)
{
	return GetRValue(cr) == GetGValue(cr) && GetGValue(cr) == GetBValue(cr);		
}
BOOL TlbbInstance::IsBlue(COLORREF cr)
{
	return GetBValue(cr) >= 0x80 && GetRValue(cr) < 0x20 && GetGValue(cr) < 0x20;
}
BOOL TlbbInstance::IsYellow(COLORREF cr)
{
	return GetRValue(cr) >= 0x80 && GetGValue(cr) >= 0x80 && GetBValue(cr) < 0x20;
}