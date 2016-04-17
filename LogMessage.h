#pragma once

#include <Windows.h>
#include <string>
#include <fstream>
#include "NetworkConnection.h"
using namespace std;


#define LOG_LEVEL_DEBUG 1
#define DEBUG_PREFIX "DEBUG"
#define LOG_LEVEL_INFORMATIONAL 2
#define INFORMATIONAL_PREFIX "INFORMATIONAL"
#define LOG_LEVEL_NOTICE 4
#define NOTICE_PREFIX "NOTICE"
#define LOG_LEVEL_WARNING 8
#define WARNING_PREFIX "WARNING"
#define LOG_LEVEL_ERROR 16
#define ERROR_PREFIX "ERROR"
#define LOG_LEVEL_CRITICAL 32
#define CRITICAL_PREFIX "CRITICAL"
#define LOG_LEVEL_ALERT 64
#define ALERT_PREFIX "ALERT"
#define LOG_LEVEL_EMERGENCY 128
#define EMERGENCY_PREFIX "EMERGENCY"

//so we know where to send strings like error message
class LogOutput
{
public:
	bool useTimeStamp;
	bool useSeverityString;
	string AddPrefixes(int severity)
	{
		string output = "";
		if (useTimeStamp)
			output += GetTimeStamp();
		if (useSeverityString)
			output += (" " + GetStringFromSeverity(severity) + ": ");
		return output;
	}

	static string GetStringFromSeverity(int severity);
	static int GetValueFromSeverityString(string severity);
	string GetTimeStamp();
	virtual void WriteMessage(int severity, string message) = 0;
};

class ConsoleOutput : public LogOutput
{
public:
	//ConsoleOutput();//create a new console window for output
	//~ConsoleOutput();
	void WriteMessage(int severity, string message);
};

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

class WindowsEventLogOutput : public LogOutput
{
	WORD ConvertSeverityToWindows(int severity);
public:
	void WriteMessage(int severity, string message);
};

class TCPOutput : public LogOutput
{
	NetworkConnection *conn;
	int socket;
public:
	TCPOutput()
	{
		conn = NULL;
		socket = -1;
	}
	TCPOutput(NetworkConnection *c, int s)
	{
		conn = c;
		socket = s;
	}
	void ChangeSocketIndex(int index)
	{
		if(index > -1)
			socket = index;
	}
	void WriteMessage(int severity, string message);
};