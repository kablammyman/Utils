#include "ConsoleOutput.h"
#include <iostream>

void ConsoleOutput::WriteMessage(int severity, string message)
{
	cout << AddPrefixes(severity) << message << endl;
}
