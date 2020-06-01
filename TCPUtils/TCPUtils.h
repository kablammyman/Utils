#ifndef INC_NETWORKCONNECTION_H
#define INC_NETWORKCONNECTION_H

#if _WIN32
#define _WIN32_WINNT 0x501

#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#endif


#include <iostream>
#include <vector>
#include <string>

#define DEFAULT_BUFLEN 512
#define NETWORK_ERROR -1
#define NETWORK_OK     0
#define GOT_CONNECTION 1
#define MAX_STRING_LENGTH 256

using namespace std;
/*
change it so that each "connection" (aka server or client) has its own
params including port num

have an array of that class*/
struct RemoteComputerConnection
{
	SOCKET theSocket;
	int portNumber = 0;
	string ipAddy;
	struct addrinfo *remoteInfo;
	//if needed, we can set an id for who or what this is connected to
	int id = 0;
};

class TCPUtils
{
protected:
	struct addrinfo *servinfo;
	struct addrinfo myInfo;
	WORD sockVersion;
	WSADATA wsaData;
	
	int sockfd;
	struct timeval tv;

	fd_set master;   // master file descriptor list
	fd_set read_fds; // temp file descriptor list for select()
	
public:
	enum SOCKET_TYPE
	{
		DATAGRAM_SOCKET = 0,
		STREAM_SOCKET 
	};

	void ReportError(int errorCode, std::string  whichFunc);
	int FillTheirInfo(addrinfo *who, SOCKET daSocket);
	
	
	
	int SendDataTCP(SOCKET daSocket, const char *msg);
	int GetDataTCP(SOCKET daSocket, char *msg, int dataSize);
	

	//for datagram sockets
	int SendDataUDP(SOCKET daSocket, const char *msg, addrinfo *whomToSend);
	int GetDataUDP(SOCKET daSocket, char *msg);

	
	bool HasRecivedData(SOCKET daSocket);
	void CloseConnection(SOCKET daSocket);
};

#endif //INC_NETWORKCONNECTION_H
