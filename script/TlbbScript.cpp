#include "stdafx.h"
#include "TlbbScript.h"

#include "UDPBroadcast.h"

void TlbbScript::Start()
{
	if(!m_start)
	{
		m_start = true;
		m_timerCount = 0;
		m_hWnd = GetForegroundWindow();
		m_hDC = GetDC(m_hWnd);
		Beep(987, 100);
		DoStart();
	}
	else
	{
		m_timerCount = 0;
		Beep(987, 100);
		DoReload();
	}
}
void TlbbScript::Stop()
{
	if(m_start)
	{
		m_start = false;

		ReleaseDC(m_hWnd, m_hDC);
		Beep(879, 100);
		DoStop();
	}
}
void TlbbScript::OnTimer()
{
	if(m_start)
	{
		m_timerCount++;
		DoOnTimer();
	}
}

BOOL TlbbScript::InitScript()
{
	g_pbc = new UDPBroadcast(1822);

	::CoInitialize(NULL);
	g_obj.CreateInstance(__uuidof(QMFunction));
	return !(g_obj == NULL);
}

HANDLE TlbbScript::GetEventHandle() 
{
	return g_pbc->GetEventHandle();
}