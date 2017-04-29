#include "Windows.h"
#include "WindowsEventLogOutput.h"

void WindowsEventLogOutput::WriteMessage(int severity, string message)
{
	char temp[1023] = { 0 };
	WORD wType = ConvertSeverityToWindows(severity);

	HANDLE hEventSource;
	LPSTR lpszStrings[1];

	if (message.length() > _countof(temp)) {
		message = message.substr(0, _countof(temp) - 10);
		message += "...";
	}


	sprintf_s(temp, _countof(temp), "%s", message.c_str());
	lpszStrings[0] = temp;

	// Get a handle to use with ReportEvent().
	//hEventSource = RegisterEventSource(0, sSourceName.c_str());
	hEventSource = RegisterEventSource(0, "SmartCatalogue");
	if (hEventSource != 0)
	{
		// Write to event log.
		ReportEvent(
			hEventSource,						//event source
			wType,								//event type
			0,									//category - none
			0x00000001L,						//event id
			0,								//used Sid
			1,									//num strings
			0,									//data size
			(LPCTSTR*)&lpszStrings[0],			//strings
			0);								//raw data
		DeregisterEventSource(hEventSource);
	}

	return;
}

WORD WindowsEventLogOutput::ConvertSeverityToWindows(int severity)
{
	switch (severity)
	{
	case LOG_LEVEL_INFORMATIONAL:
	case LOG_LEVEL_NOTICE:
		return EVENTLOG_INFORMATION_TYPE;

	case LOG_LEVEL_WARNING:
		return EVENTLOG_WARNING_TYPE;

	case LOG_LEVEL_ERROR:
	case LOG_LEVEL_CRITICAL:
	case LOG_LEVEL_ALERT:
	case LOG_LEVEL_EMERGENCY:
		return EVENTLOG_ERROR_TYPE;
	default:
		return EVENTLOG_INFORMATION_TYPE;
	}
}