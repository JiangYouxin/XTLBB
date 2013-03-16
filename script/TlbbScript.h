#ifndef TLBB_SCRIPT_H
#define TLBB_SCRIPT_H

#include "TlbbInstance.h"
#import "C:\\Windows\\QMDispatch.dll" no_namespace

class UDPBroadcast;
class TlbbScript: public TlbbInstance
{
public:
	TlbbScript() { m_start = false; }
	void Start();
	void Stop();
	void OnTimer();
	virtual char * GetName() { return ""; }
protected:
	virtual void DoStart() {}
	virtual void DoStop() {}
	virtual void DoReload() {}
	virtual void DoOnTimer() {}
public:
	static BOOL InitScript();
	static HANDLE GetEventHandle(); 
protected:
	bool m_start;
	int m_timerCount;
	static IQMFunctionPtr g_obj;
public:
	static UDPBroadcast * g_pbc;
};

#endif
