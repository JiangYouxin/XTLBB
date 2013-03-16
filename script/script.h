// script.h
#pragma once


#include "UDPBroadcast.h"

extern HWND g_hWnd;

#pragma comment(lib, "winmm.lib")


extern DWORD g_bServer;
extern DWORD g_bSaveBmp;
extern DWORD g_bWhite;
extern DWORD g_bGamer;
extern char g_sName[256];

char g_dllName[] = "C:\\InjectDLL.dll";


class SimpleScript: public ScriptBase
{
protected:
	virtual void DoStart()
	{
		m_fast = false;
	}
	virtual void DoReload()
	{
		m_fast = !m_fast;
	}
	virtual void DoOnTimer()
	{
		DWORD t = m_fast ? 1 : 10;
		if(m_timerCount == 1)
			SendKeyPress(m_hWnd, 122);
		else if(m_timerCount == t + 1)
			SendKeyPress(m_hWnd, 123);
		if(m_timerCount == 2 * t)
			m_timerCount = 0;
	}
public:
	virtual char * GetName()
	{
		if(!m_start)
			return "简单打怪";
		else if(m_fast)
			return "简单打怪(快速)";
		else
			return "简单打怪(匀速)";
	}
protected:
	DWORD m_fast;
};

/*
class NormalScript: public ScriptBase
{
protected:
	virtual void DoStart()
	{
		m_auto = true;
		m_n = 0;
	}
	virtual void DoReload()
	{
		m_auto = !m_auto;
		m_n = 0;
	}
	virtual void DoOnTimer()
	{
		// 看色打怪F11之后，以及每次发招之后，等待时间为50毫秒
		if(m_timerCount <= 5) return;

		OBJECT_COLOR oc = GetObjectColor();
		if((oc == GREEN_OBJECT && !m_auto || oc == RED_OBJECT && m_auto) && !IsGamer() && IsAlive())
		{
			if(m_n < 80)
			{
				if(m_n % 40 == 0) 
					SendKeyPress(m_hWnd, 123);						
				m_n++;
				m_timerCount = 1;
				return;		
			}
		}
		SendKeyPress(m_hWnd, 122);
		m_n = 0;
		m_timerCount = 1;
	}
public:
	virtual char * GetName()
	{
		if(!m_start)
			return "看色打怪";
		else if(m_auto)
			return "看色打怪(只打红怪)";
		else
			return "看色打怪(只打绿怪)";
	}
protected:
	bool m_auto;
	DWORD m_n;
};
*/

class MouseScript: public ScriptBase
{
protected:
	virtual void DoStart()
	{
		GetCursorPos(&m_p);
	}
	virtual void DoOnTimer()
	{
		if(m_timerCount % 100 == 1 && m_timerCount < 900)
		{
			SendKeyPress(m_hWnd, 122);
		}
		else if(m_timerCount % 50 == 11 && m_timerCount < 950)
		{
			SendKeyPress(m_hWnd, 123);
		}
		else if(m_timerCount == 981)
		{
			SetCursorPos(m_p.x, m_p.y);
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		}
		else if(m_timerCount == 1001)
		{
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
		else if(m_timerCount == 1400)
		{
			m_timerCount = 0;
		}
	}
	virtual char * GetName()
	{
		return "引怪回原点";
	}
	POINT m_p;
};
class AIScript: public ScriptBase
{
protected:
	virtual void DoStart()
	{
		StartInject(m_hWnd);

		if(!g_bSaveBmp)
		{
			for(int i = 0; i < m_aBmp.GetSize(); i++)
				DeleteObject(m_aBmp[i]);
			m_aBmp.RemoveAll();
		}
		m_mode = 0;
		m_fast = false;
		m_fastCount = 0;
		m_objectCount = 0;
		m_n = 0;
		m_xxxCount = 0;

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
	}
	virtual void DoStop()
	{
		StopInject(m_hWnd);
	}
	virtual void DoReload()
	{
		m_mode = (m_mode + 1) % 3;
		m_n = 0;
	}
	virtual void DoOnTimer()
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

		/*
		if(g_bWhite)
		{
			if(oc == WHITE_OBJECT && !IsGamer() && IsAlive())
			{
				m_fastCount = 0;
				if(!m_fast)
				{
					m_fast = true;
					char buf[256] = "有人抢怪: ";
					lstrcat(buf, g_sName);
					g_pbc->SendBroadcast(buf, strlen(buf) + 1);
				}
			}
		}
		if(g_bGamer && IsGamer())
		{
			m_gamerCount = 0;
			if(!m_gamer)
			{
				m_gamer = true;
				char buf[256] = "切到玩家: ";
				lstrcat(buf, g_sName);
				g_pbc->SendBroadcast(buf, strlen(buf) + 1);
			}			
		}
		*/
		if((oc != INVALID_OBJECT) /*&& !IsGamer()*/ && IsAlive())
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
			if(m_n == 0)
			{
				/*
				m_objectCount++;
				
				HDC hdc = CreateCompatibleDC(m_hDC);
				HBITMAP hbmp = CreateCompatibleBitmap(m_hDC, 40, 40);
				SelectObject(hdc, hbmp);
				BitBlt(hdc, 0, 0, 40, 40, m_hDC, 420, 4, SRCCOPY);
				DeleteDC(hdc);
					
				if(!FindBitmap(hbmp))
				{
					if(m_objectCount <= 20 || IsBoss())
						AddBitmap(hbmp);
					else
					{
						DeleteObject(hbmp);
						goto another;
					}
				}
				else 
					DeleteObject(hbmp);
				*/
				// goto another;
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
	/*
	bool FindBitmap(HBITMAP hbmp)
	{
		HDC hdc1 = CreateCompatibleDC(m_hDC);
		SelectObject(hdc1, hbmp);
		
		bool b1 = false;
		for(int n = 0; n < m_aBmp.GetSize() && !b1; n++)
		{
			bool b2 = false;
			HDC hdc2 = CreateCompatibleDC(m_hDC);
			SelectObject(hdc2, m_aBmp[n]);
			for(int i = 0; i < 40 && !b2; i++) for(int j = 0; j < 40 && !b2; j++)
			{
				if(GetPixel(hdc1, i, j) != GetPixel(hdc2, i, j))
					b2 = true;		
			}
			DeleteDC(hdc2);
			b1 = !b2;
		}
		DeleteDC(hdc1);
		return b1;
	}
	void AddBitmap(HBITMAP hbmp)
	{
		m_aBmp.Add(hbmp);
	}
	*/
public:
	virtual char * GetName()
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
protected:
	DWORD m_mode;
	bool m_fast;
	DWORD m_fastCount; 
	bool m_gamer;
	DWORD m_gamerCount;
	DWORD m_n;
	DWORD m_objectCount;
	DWORD m_xxxCount;
	ATL::CSimpleArray<HBITMAP> m_aBmp;
};

class AIScript2: public AIScript
{
	virtual void DoOnTimer()
	{
		if (m_xxxCount % 600 == 50)
			SendKeyPress(m_hWnd, 117);
		AIScript::DoOnTimer();
	}
	virtual char * GetName()
	{
		if(!m_start)
			return "智能打怪捡包";
		else if(m_mode == 0)
			return "智能打怪捡包(自动)";
		else if(m_mode == 1)
			return "智能打怪捡包(快速切怪)";
		else 
			return "智能打怪捡包(帮打模式)";
	}
};

class GuaJiScript: public ScriptBase
{
public:
	virtual void DoOnTimer()
	{
		if(m_timerCount == 300)
		{
			if(FoundPic()) 
			{
				char buf[256] = "挂机检测: ";
				lstrcat(buf, g_sName);
				g_pbc->SendBroadcast(buf, strlen(buf) + 1);
			}
			else
				m_timerCount = 0;
		}
		else if(m_timerCount >= 6000)
			m_timerCount = 0;
	}
	virtual char * GetName()
	{
		return "防挂机检测";
	}
private:
	bool FoundPic()
	{
		int x, y;

		if(g_obj)
		{
			x = g_obj->FindPic(0, 0, 1600, 1000, "C:\\002.bmp", (float)0.7);
			y = x % 0x2000;
			x = x / 0x2000;
			return x > 0 && y > 0;
		}
		else
			return false;
	}
};

class InjectScript: public ScriptBase
{
protected:
	virtual void DoStart()
	{
		StartInject(m_hWnd);
	}
	virtual void DoStop()
	{
		StopInject(m_hWnd);
	}
	virtual char * GetName()
	{
		if(!m_start)
			return "双儿辅助";
		else 
			return "双儿辅助(开启)";
	}
};
#define WM_CHANGE_MODE (WM_USER + 0xc531)

class SimpleFarmerScript: public ScriptBase
{
protected:
	virtual void DoStart()
	{
		StartInject(m_hWnd);
		m_mode = 0;

		PostMessage(m_hWnd, WM_CHANGE_MODE, 0, 2);
	}
	virtual void DoStop()
	{
		StopInject(m_hWnd);
	}
	virtual void DoReload()
	{
		m_mode = 1;
		PostMessage(m_hWnd, WM_CHANGE_MODE, 0, 3);
	}
	virtual char * GetName()
	{
		if(!m_start)
			return "种地测试";
		else if(m_mode == 0) 
			return "种地测试(记录)";
		else
			return "种地测试(进行)";
	}
private:
	int m_mode;
};

class FarmerScript: public ScriptBase
{
protected:
	virtual void DoStart()
	{
		StartInject(m_hWnd);
		m_mode = 0;

		PostMessage(m_hWnd, WM_CHANGE_MODE, 0, 2);
	}
	virtual void DoStop()
	{
		StopInject(m_hWnd);
	}
	virtual void DoReload()
	{
		m_mode = 1;
		m_state = 0;
		m_tickCount = GetTickCount();
	}
	virtual void DoOnTimer()
	{
		if (m_mode == 0)
			return;

		DWORD t = GetTickCount() - m_tickCount;

		if (m_state == 0 && t >= 330 * 1000)
		{
			// 开始收第一块地
			m_state = 1;
		}
		else if (m_state == 1 && t >= 330 * 1000 + 3 * 1000)
		{
			// 完成收第一块地
			m_state = 2;
		}
		else if (m_state == 2 && t >= 330 * 1000 + 4 * 1000)
		{
			// 开始收第二块地
			m_state = 3;
		}
		else if (m_state == 3 && t >= 330 * 1000 + 7 * 1000)
		{
			// 完成收第二块地
			m_state = 4;
		}
		else if (m_state == 4 && t >= 330 * 1000 + 8 * 1000)
		{
			// 开始收第三块地
			m_state = 5;
		}
		else if (m_state == 5 && t >= 330 * 1000 + 11 * 1000)
		{
			// 完成收第三块地
			m_state = 6;
		}
		else if (m_state == 6 && t >= 330 * 1000 + 12 * 1000)
		{
			// 开始收第四块地
			m_state = 7;
		}
		else if (m_state == 7 && t >= 330 * 1000 + 15 * 1000)
		{
			// 完成收第四块地
			m_state = 8;
		}
		else if (m_state == 8 && t >= 330 * 1000 + 16 * 1000)
		{
			// 种下一次地
			PostMessage(m_hWnd, WM_CHANGE_MODE, 0, 3);
			DoReload();
		}
	}
	virtual char * GetName()
	{
		if(!m_start)
			return "种地";
		else if(m_mode == 0) 
			return "种地(记录指令)";
		else
			return "种地(进行中)";
	}
private:
	int m_mode;
	int m_state;
	DWORD m_tickCount;
};
class ScriptFactory
{
public:
	static void Init(HWND hwnd)
	{
		ScriptBase::InitScript();
		for(int i = 0; i < 8; i++)
			scripts[i] = NULL;
		g_hwnd = hwnd;
	}
	static void CreateScript(ScriptBase * psb, int hotKey, DWORD id)
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
		if(g_bServer && ScriptBase::g_pbc->RecvBroadcast(buffer, 256, &addr) > 0)
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
	static ScriptBase * scripts[8];
	static DWORD ids[8];
	static HWND g_hwnd;
};