#include "TCPOutput.h"

void TCPOutput::WriteMessage(int severity, string message)
{
	string msg = (AddPrefixes(severity) + message + "\n");
	conn->sendData(socket, msg.c_str());
}
