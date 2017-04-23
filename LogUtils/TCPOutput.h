#pragma once
#include "LogMessage.h"

#include "TCPUtils.h"

class TCPOutput : public LogOutput
{
	TCPUtils *conn;
	int socket;
public:
	TCPOutput()
	{
		conn = NULL;
		socket = -1;
	}
	TCPOutput(TCPUtils *c, int s)
	{
		conn = c;
		socket = s;
	}
	void ChangeSocketIndex(int index)
	{
		if (index > -1)
			socket = index;
	}
	void WriteMessage(int severity, string message);
};