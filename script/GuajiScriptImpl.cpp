#include "stdafx.h"
#include "GuajiScriptImpl.h"
#include "UDPBroadcast.h"

extern char g_sName[256];

void GuajiScriptImpl::DoOnTimer()
{
	if(m_timerCount == 300)
	{
		if(FoundPic()) 
		{
			char buf[256] = "¹Ò»ú¼ì²â: ";
			lstrcat(buf, g_sName);
			g_pbc->SendBroadcast(buf, strlen(buf) + 1);
		}
		else
			m_timerCount = 0;
	}
	else if(m_timerCount >= 6000)
		m_timerCount = 0;
}
char * GuajiScriptImpl::GetName()
{
	return "·À¹Ò»ú¼ì²â";
}

BOOL GuajiScriptImpl::FoundPic()
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
		return FALSE;
}
