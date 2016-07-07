#pragma once

#include "LogMessage.h"
#include <ctime>//check execution time


int LogOutput::GetValueFromSeverityString(string severity)
{
	if (severity.compare(DEBUG_PREFIX) == 0)
		return LOG_LEVEL_DEBUG;

	else if (severity.compare(INFORMATIONAL_PREFIX) == 0)
		return LOG_LEVEL_INFORMATIONAL;

	else if (severity.compare(NOTICE_PREFIX) == 0)
		return LOG_LEVEL_NOTICE;

	else if (severity.compare(WARNING_PREFIX) == 0)
		return LOG_LEVEL_WARNING;

	else if (severity.compare(ERROR_PREFIX) == 0)
		return LOG_LEVEL_ERROR;

	else if (severity.compare(EMERGENCY_PREFIX) == 0)
		return LOG_LEVEL_EMERGENCY;

	else if (severity.compare(CRITICAL_PREFIX) == 0)
		return LOG_LEVEL_CRITICAL;

	else if (severity.compare(ALERT_PREFIX) == 0)
		return LOG_LEVEL_ALERT;


	//if we get here, then we dont have a severity level keyword
	return 0;
}

//-------------------------------------------------------------------------------------
string LogOutput::GetStringFromSeverity(int severity)
{
	switch (severity)
	{
	case LOG_LEVEL_DEBUG:
		return DEBUG_PREFIX;
	case LOG_LEVEL_INFORMATIONAL:
		return INFORMATIONAL_PREFIX;
	case LOG_LEVEL_NOTICE:
		return NOTICE_PREFIX;
	case LOG_LEVEL_WARNING:
		return WARNING_PREFIX;
	case LOG_LEVEL_ERROR:
		return ERROR_PREFIX;
	case LOG_LEVEL_CRITICAL:
		return CRITICAL_PREFIX;
	case LOG_LEVEL_ALERT:
		return ALERT_PREFIX;
	case LOG_LEVEL_EMERGENCY:
		return EMERGENCY_PREFIX;
	}
	return DEBUG_PREFIX;
}

//-------------------------------------------------------------------------------------
string LogOutput::GetTimeStamp()
{
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	return asctime(timeinfo);
	//printf("The current date/time is: %s", asctime(timeinfo));
}

string LogOutput::GetTimeStampFromMilis(double milis)
{
	int seconds = milis / 1000;
	int minutes = seconds / 60;
	seconds %= 60;
	int hours = minutes / 60;
	minutes %= 60;
	int days = hours / 24;
	hours %= 24;
	string output;

	if (days > 0)
		output += (to_string(days) + " days ");
	if (hours > 0)
		output += (to_string(hours) + " hours ");
	if (minutes > 0)
		output += (to_string(minutes) + " minutes ");
	if (seconds > 0)
		output += (to_string(seconds) + " seconds");

	return output;
}