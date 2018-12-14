#pragma once
#include "LogMessage.h"
#include <Windows.h>
class WindowsEventLogOutput : public LogOutput
{
	WORD ConvertSeverityToWindows(int severity);
public:
	void WriteMessage(int severity, string message);
};
