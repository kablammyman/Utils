#ifndef INC_NETWORKCONNECTION_H
#define INC_NETWORKCONNECTION_H

#if _WIN32
#include <Windows.h>
#include <winsock.h>
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
	SOCKADDR_IN remoteInfo;
	//if needed, we can set an id for who or what this is connected to
	int id = 0;
};

class TCPUtils
{
	LPHOSTENT hostEntry;
	SOCKADDR_IN myInfo;
	
	WORD sockVersion;
	WSADATA wsaData;
	
	int sockfd;
	struct timeval tv;

	fd_set master;   // master file descriptor list
	fd_set read_fds; // temp file descriptor list for select()
	
	bool waitingForClients = false;
	SOCKET listeningSocket;
	int numListeningConnections;
	vector<RemoteComputerConnection> remoteConnections;

public:
	enum SOCKET_TYPE
	{
		DATAGRAM_SOCKET = 0,
		STREAM_SOCKET 
	};

	void ReportError(int errorCode, std::string  whichFunc);
	int FillTheirInfo(SOCKADDR_IN *who, SOCKET daSocket);
	
	int WaitForFirstClientConnect();
	int WaitForClientAsync();

	int StartServer(int numConnections, int port, SOCKET_TYPE socketType = STREAM_SOCKET);
	int ConnectToServer(string ip, int port, SOCKET_TYPE socketType = STREAM_SOCKET);
	void Shutdown();
	
	
	//for stream sockets
	int ServerBroadcast(const char *msg);

	int SendData(int socketIndex, const char *msg);
	int GetData(int socketIndex, char *msg, int dataSize);
	

	//for datagram sockets
	int SendData(SOCKET daSocket, const char *msg, SOCKADDR_IN whomToSend);
	int GetData(SOCKET daSocket, char *msg, SOCKADDR_IN whosSendingMeStuff);

	int ChangeToNonBlocking(SOCKET daSocket);
	size_t GetNumConnections() {return remoteConnections.size();}
	SOCKET GetSocket(int index) { return remoteConnections[index].theSocket; }
	bool HasRecivedData(int index);
	void CloseConnection(int index);
};


#endif //INC_NETWORKCONNECTION_H
