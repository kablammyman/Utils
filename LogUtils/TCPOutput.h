#pragma once
#include "LogMessage.h"

#include "NetUtils.h"

class TCPOutput : public LogOutput
{
	NetUtils *conn;
	int socket;
public:
	TCPOutput()
	{
		conn = 0;
		socket = -1;
	}
	TCPOutput(NetUtils *c, int s)
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