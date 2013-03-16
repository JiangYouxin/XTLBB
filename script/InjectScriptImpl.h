#ifndef INJECT_SCRIPT_IMPL_H
#define INJECT_SCRIPT_IMPL_H

#include "TlbbScript.h"

class InjectScriptImpl: public TlbbScript
{
protected:
	virtual void DoStart();
	virtual void DoStop();
public:
	virtual char * GetName();
};

#endif