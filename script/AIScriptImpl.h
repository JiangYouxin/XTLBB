#ifndef AI_SCRIPT_IMPL_H
#define AI_SCRIPT_IMPL_H

#include "TlbbScript.h"

class AIScriptImpl: public TlbbScript
{
protected:
	virtual void DoStart();
	virtual void DoStop();
	virtual void DoReload();
	virtual void DoOnTimer();
public:
	virtual char * GetName();
private:
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

#endif