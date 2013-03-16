#ifndef GUAJI_SCRIPT_IMPL_H
#define GUAJI_SCRIPT_IMPL_H

#include "TlbbScript.h"

class GuajiScriptImpl: public TlbbScript
{
protected:
	virtual void DoOnTimer();
public:
	virtual char * GetName();
private:
	BOOL FoundPic();
};

#endif