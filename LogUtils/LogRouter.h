#pragma once
#include <vector>

#include "LogMessage.h"

#define CONSOLE_OUTPUT   1
#define CONSOLE_OUTPUT_STRING	"CONSOLE"
#define FILE_OUTPUT      2
#define FILE_OUTPUT_STRING "TEXTFILE"
#define TCP_OUTPUT       4
#define TCP_OUTPUT_STRING "TCP"
#define EVENT_LOG_OUTPUT 8
#define EVENT_LOG_OUTPUT_STRING "EVENTLOG"

#define NUM_MESSAGE_TYPES 4
#define MAX_LOG_ENTITIES 4

//an array of these are filled with the data from each "blob" of the cfg
struct LogEntity
{
	LogOutput *logOut;
	int logClassId;      //what type of logs are we listening for
	int id; //the id...assigned by the order itwas found
	string type; //more for debugging
};


class LogRouter
{
	//this will be the array of all the types of log messages we are lookging for. this will be specified in the cfg, and created after thats parsed
	//similar to the above, but this is for the message types

	int logCaptureFlags; //what type of log messages are we listening to specified in the cfg
	int logSeverityFlags;    //where are we dispaying the log message? concole? logfile? 
	int scanLogFlags; //all teh tye of scan log results we can have. we check these since these are special

	std::vector<LogEntity *> severityLookupTable[NUM_MESSAGE_TYPES];
	/* to help visualize what this does, think of this as a table of all the differnt severity tags.
	each log entity goes into each slot that is specified in the seveirty string

	so, if a log entity says it wants to watch for notice and warning, then that log entity's id will be
	in both of those elements.
	+================+===============+========+================+=================+==========+=======+===========+
	|     debug      | informational | notice |    warning     |      error      | critical | alert | emergency |
	+================+===============+========+================+=================+==========+=======+===========+
	|                |               |        | logEntity->id1 | logEntity->id1  |          |       |           |
	+----------------+---------------+--------+----------------+-----------------+----------+-------+-----------+
	| logEnitiy->id2 |               |        |                |                 |          |       |           |
	+----------------+---------------+--------+----------------+-----------------+----------+-------+-----------+
	|                |               |        |                | logEntitiy->id3 |          |       |           |

	so everytime we get a log message of type warning, we will go thru the list of ids (in this case, theres just 1 in the vector)
	each enitity has its own way to do output, and this is how all the messages will end up where you want them with very lil branching logic
	*/
	vector<LogEntity *> logs;

	void InitLogEntities();
	void ClearLogEntities();

	inline bool containsFlag(int var, int flag)
	{
		if ((var & flag) == flag)
			return true;
		return false;
	}
	inline int getArrayIndex(int flag)
	{
		int counter = 0;
		while (flag > 0)
		{
			flag >>= 1;
			counter++;
		}
		if (counter > 0)
			return counter - 1;
		return 0;
	}

	void AddLoggerSeverityDetails(vector<string> severityVec, LogEntity *logger);

public:
	LogRouter();
	~LogRouter();
	
	void Log(int logSeverity, string msg);
	LogEntity * AddLogger(LogOutput *newLog, vector<string> severityTokens);
	LogEntity * AddLogger(LogOutput *newLog, string severityToken);
};
