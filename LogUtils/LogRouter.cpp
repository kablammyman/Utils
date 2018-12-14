/*this class will help make our logging easily configurabkle, witout changing whats already in place
Right now, we will use slow methods to log things, but in teh future a lib can be used in its place.
*/

#include "LogRouter.h"
#include "StringUtils.h"

#include "ConsoleOutput.h"
#include "LogfileOutput.h"
#include "LogMessage.h"

#if _WIN32
//#include "TCPOutput.h"
#include "WindowsEventLogOutput.h"
#endif

LogRouter::LogRouter()
{
	InitLogEntities();
}
//-------------------------------------------------------------------------------------
LogRouter::~LogRouter()
{
	ClearLogEntities();
}

void LogRouter::AddLoggerSeverityDetails(vector<string> severityVec, LogEntity *logger)
{
	//add a pointer to this new obj to each list that we want to notify when the proper log message is processed
	for (size_t i = 0; i < severityVec.size(); i++)
	{
		int logSeverityValue = LogOutput::GetValueFromSeverityString(severityVec[i]);//take the cfg string amd get the log severity val
		int severityIndex = getArrayIndex(logSeverityValue); //find out where in the array we need to place the newlog obj
		severityLookupTable[severityIndex].push_back(logger);//place obj in array index chain.
		logSeverityFlags |= logSeverityValue;//set bit flag
	}
}
//-------------------------------------------------------------------------------------
//can be used to add a logger from code outside of the cfg file
LogEntity * LogRouter::AddLogger(LogOutput *newLog, vector<string> severityTokens)
{
	int id = (int)logs.size();
	
	LogEntity *newLogEntity = new LogEntity();
	newLogEntity->id = id;
	newLogEntity->logOut = newLog;
	newLogEntity->logOut->useTimeStamp = false;
	newLogEntity->logOut->useSeverityString = false;

	logs.push_back(newLogEntity);
	AddLoggerSeverityDetails(severityTokens, newLogEntity);

	return newLogEntity;
}
//-------------------------------------------------------------------------------------
LogEntity * LogRouter::AddLogger(LogOutput *newLog, string severityToken)
{
	int id = (int)logs.size();

	LogEntity *newLogEntity = new LogEntity();
	newLogEntity->id = id;
	newLogEntity->logOut = newLog;
	newLogEntity->logOut->useTimeStamp = false;
	newLogEntity->logOut->useSeverityString = false;

	logs.push_back(newLogEntity);
	vector<string> severityTokens = StringUtils::Tokenize(severityToken,",");
	AddLoggerSeverityDetails(severityTokens, newLogEntity);

	return newLogEntity;
}
//-------------------------------------------------------------------------------------
void LogRouter::InitLogEntities()
{
	logCaptureFlags = 0;
	logSeverityFlags = 0;
	fill(severityLookupTable, severityLookupTable + NUM_MESSAGE_TYPES, vector<LogEntity *>());
}
//-------------------------------------------------------------------------------------
void LogRouter::ClearLogEntities()
{
	while (!logs.empty())
	{
		delete logs.back();
		logs.pop_back();
	}

	logs.clear();//prob not needed

	for (int i = 0; i < NUM_MESSAGE_TYPES; i++)
		severityLookupTable[i].clear();
}
//-------------------------------------------------------------------------------------
void LogRouter::Log(int logSeverity, string msg)
{
	//see if we are checking for this type of message
	if (!containsFlag(logSeverityFlags, logSeverity))
		return;
	//find out where the message is going...all destinations are stored in a single array
	int severityIndex = getArrayIndex(logSeverity);

	//for each destination, loop thru all loggers that are set for said destination
	size_t numOutputsForCurSeverity = severityLookupTable[severityIndex].size();
	for (size_t i = 0; i < numOutputsForCurSeverity; i++)
	{
		LogEntity * curLogger = severityLookupTable[severityIndex][i];//the specific logger
		curLogger->logOut->WriteMessage(logSeverity, msg);//print the message

	}

}
//-------------------------------------------------------------------------------------
LogOutput * LogRouter::LogOutputFactory(string logType, string loggerDetails)
{
	if (logType.compare(CONSOLE_OUTPUT_STRING) == 0)
		return new ConsoleOutput();

	else if (logType.compare(FILE_OUTPUT_STRING) == 0)
		return new LogFileOutput(loggerDetails);

	#if _WIN32
	//else if (logType.compare(TCP_OUTPUT_STRING) == 0)
	//	return new TCPOutput();

	else if (logType.compare(EVENT_LOG_OUTPUT_STRING) == 0)
		return new WindowsEventLogOutput();
	#endif


	//uf we get here, we got an invalid type.
	//instad of returning null, return a console output
	return new ConsoleOutput();
}