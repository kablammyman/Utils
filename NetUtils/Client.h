#pragma once
#include "NetUtils.h"

class Client: public NetUtils
{
public:
	struct ServerConnection
	{
		SOCKET sendSocket;
		//SOCKET recvSocket;//for udp sockets...mistly to test using1machine so i can use more than 1 port
		string ipAddy;//srevers uip
		struct addrinfo *remoteInfo;
	};

	int ConnectToServer(const char *ip, const char* serverPort,/*const char* listenPort,*/  SOCKET_TYPE socketType = STREAM_SOCKET);
	int SendDataTCP(const char *msg);//for stream sockets
	int GetDataTCP(unsigned char *msg, int dataSize);//for stream sockets

	int SendDataUDP(const char *msg, int dataSize);//for datagram sockets
	int GetDataUDP(unsigned char *msg);//for datagram sockets

	bool HasRecivedData();

	//this needs to send a packet to tell the server we are leaving
	void DisconnectFromServer();
	int ChangeToIsBlocking(bool isBlocking);

	private:
		ServerConnection serverConnection;
		//hostent hostEntry;

};