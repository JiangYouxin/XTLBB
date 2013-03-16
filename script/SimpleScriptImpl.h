#ifndef SIMPLE_SCRIPT_IMPL_H
#define SIMPLE_SCRIPT_IMPL_H

#include "TlbbScript.h"

class SimpleScriptImpl: public TlbbScript
{
protected:
	virtual void DoStart() { m_bFast = false; }
	virtual void DoReload() { m_bFast = !m_bFast; }
	virtual void DoOnTimer();
public:
	virtual char * GetName();
private:
	BOOL m_bFast;
};

#endif