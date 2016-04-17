/*this class will help make our logging easily configurabkle, witout changing whats already in place
Right now, we will use slow methods to log things, but in teh future a lib can be used in its place.
*/

#include "LogRouter.h"
#include "Windows.h"
#include "Utils.h"
#include "CFGReaderDll.h"

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
	for (int i = 0; i < severityVec.size(); i++)
	{
		int logSeverityValue = LogOutput::GetValueFromSeverityString(severityVec[i]);//take the cfg string amd get the log severity val
		int severityIndex = getArrayIndex(logSeverityValue); //find out where in the array we need to place the newlog obj
		severityLookupTable[severityIndex].push_back(logger);//place obj in array index chain.
		logSeverityFlags |= logSeverityValue;//set bit flag
	}
}
//-------------------------------------------------------------------------------------
void LogRouter::GetLoggingInfoFromCFG()
{
	ClearLogEntities();
	

	vector<string> loggerList = CFG::CFGReaderDLL::getCfgListValue("LOGGERS");

	for (size_t i = 0; i < loggerList.size(); i++)
	{
		string listName = loggerList[i];
		vector<string> curLogger = CFG::CFGReaderDLL::getCfgListValue(listName);

		string loggingType = CFG::CFGReaderDLL::getStringValueFromList(listName, "TYPE");//consle, logfile syslog etc
		string severityLevelString = CFG::CFGReaderDLL::getStringValueFromList(listName, "SEVERITY"); //should have all the logging level values this appender wants
		
		//optional, so far, only textfiles have this data
		string loggerDetails = CFG::CFGReaderDLL::getStringValueFromList(listName, "PATH");

		LogEntity * temp = AddLogger(loggingType, severityLevelString, loggerDetails);
		if(CFG::CFGReaderDLL::getCfgBoolValue("ADD_TIMESTAMP"))
			temp->logOut->useTimeStamp = true;
		if (CFG::CFGReaderDLL::getCfgBoolValue("ADD_SEVERITY"))
			temp->logOut->useSeverityString = true;
	}

}
//-------------------------------------------------------------------------------------
//an be used to add a logger from code outside of the cfg file
LogEntity * LogRouter::AddLogger(string type, string severityTypes, string details)
{
	int id = (int)logs.size() +1;
	vector<string> severityTokens = Utils::tokenize(severityTypes, ",");

	LogEntity *newLog = new LogEntity();
	newLog->id = id;
	newLog->logOut = LogOutputFactory(type, details);
	newLog->type = type;
	newLog->logOut->useTimeStamp = false;
	newLog->logOut->useSeverityString = false;
	logs.push_back(newLog);
	AddLoggerSeverityDetails(severityTokens, newLog);

	return newLog;
}

//-------------------------------------------------------------------------------------
//an be used to add a logger from code outside of the cfg file
LogEntity * LogRouter::AddLogger(LogOutput *newLog, string severityTypes)
{
	int id = (int)logs.size();
	vector<string> severityTokens = Utils::tokenize(severityTypes, ",");

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
void LogRouter::InitLogEntities()
{
	logCaptureFlags = 0;
	logSeverityFlags = 0;
	fill(severityLookupTable, severityLookupTable + NUM_MESSAGE_TYPES, vector<LogEntity *>());
}

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

LogOutput * LogRouter::LogOutputFactory(string logType, string loggerDetails)
{
	if (logType.compare(CONSOLE_OUTPUT_STRING) == 0)
		return new ConsoleOutput();

	else if (logType.compare(FILE_OUTPUT_STRING) == 0)
		return new LogFileOutput(loggerDetails);

	else if (logType.compare(TCP_OUTPUT_STRING) == 0)
		return new TCPOutput();

	else if (logType.compare(EVENT_LOG_OUTPUT_STRING) == 0)
		return new WindowsEventLogOutput();


	//uf we get here, we got an invalid type.
	//instad of returning null, return a console output
	return new ConsoleOutput();
}

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