#include "LogfileOutput.h"
#include <iostream>

void LogFileOutput::WriteMessage(int severity, string message)
{
	fs << AddPrefixes(severity) << message << endl;
}
