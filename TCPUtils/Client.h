#pragma once
#include "TCPUtils.h"

class Client: public TCPUtils
{
public:
	struct ServerConnection
	{
		SOCKET theSocket;
		int portNumber = 0;
		string ipAddy;
		struct addrinfo *remoteInfo;
	};

	int ConnectToServer(const char *ip, const char* port, SOCKET_TYPE socketType = STREAM_SOCKET);
	int SendDataTCP(const char *msg);//for stream sockets
	int GetDataTCP(char *msg, int dataSize);//for stream sockets

	int SendDataUDP(const char *msg);//for datagram sockets
	int GetDataUDP(char *msg);//for datagram sockets

	bool HasRecivedData();

	//this needs to send a packet to tell the server we are leaving
	void DisconnectFromServer();

	private:
		ServerConnection serverConnection;
		LPHOSTENT hostEntry;

};