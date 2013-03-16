#ifndef INJECT_SCRIPT_IMPL_H
#define INJECT_SCRIPT_IMPL_H

#include "TlbbScript.h"

class InjectScriptImpl: public TlbbScript
{
public:
	InjectScriptImpl();
protected:
	virtual void DoStart();
	virtual void DoStop();
public:
	virtual char * GetName();
private:
	char szDllName[MAX_PATH + 1];
};

#endif