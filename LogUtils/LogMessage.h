#pragma once

#include <Windows.h>
#include <string>
#include <fstream>

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
	static string GetTimeStamp();
	static string GetTimeStampFromMilis(double milis);
	virtual void WriteMessage(int severity, string message) = 0;
};

