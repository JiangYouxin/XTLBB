#include "stdafx.h"
#include "Log.h"

void MyLog(const char *format, ...)
{
	va_list v;
	va_start(v, format);

	char buf[1024];
	_vsnprintf_s(buf, 1024, format, v);

	OutputDebugStringA(buf);
}