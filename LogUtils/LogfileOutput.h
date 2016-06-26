#pragma once
#include <fstream>
#include "LogMessage.h"

using namespace std;

class LogFileOutput : public LogOutput
{
	fstream fs;
public:
	LogFileOutput(string testFile)
	{
		fs.open(testFile, fstream::in | fstream::out | fstream::app);
	}

	~LogFileOutput()
	{
		fs.close();
	}

	void WriteMessage(int severity, string message);
};