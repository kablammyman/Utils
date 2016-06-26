#pragma once
#include "LogMessage.h"

#include "NetworkConnection.h"

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
		if (index > -1)
			socket = index;
	}
	void WriteMessage(int severity, string message);
};