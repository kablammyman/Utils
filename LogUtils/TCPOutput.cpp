#include "TCPOutput.h"

void TCPOutput::WriteMessage(int severity, string message)
{
	string msg = (AddPrefixes(severity) + message + "\n");
	conn->SendDataTCP(socket, msg.c_str());
}
