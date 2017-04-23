#pragma once

#include "LogMessage.h"

class ConsoleOutput : public LogOutput
{
public:
	//ConsoleOutput();//create a new console window for output
	//~ConsoleOutput();
	void WriteMessage(int severity, string message);
};