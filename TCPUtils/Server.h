#pragma once
#include "TCPUtils.h"

class Server : public TCPUtils
{
	bool waitingForClients = false;
	SOCKET listeningSocket;
	int numListeningConnections;
	vector<RemoteComputerConnection> remoteConnections;
	
public:
	int SendDataTCP(int socketIndex, const char *msg);//for stream sockets
	int GetDataTCP(int socketIndex, char *msg, int dataSize);//for stream sockets

	int SendDataUDP(int socketIndex, const char *msg);//for datagram sockets
	int GetDataUDP(int socketIndex, char *msg);//for datagram sockets

	int ServerBroadcast(const char *msg);
	int HasRecivedData();
	
	
	int StartServer(int numConnections, char* port, SOCKET_TYPE socketType = STREAM_SOCKET);
	
	int WaitForFirstTCPClientConnect();
	int WaitForTCPClientAsync();
	//for stream sockets
	
	int ChangeToNonBlocking(SOCKET daSocket);
	size_t GetNumConnections() {return remoteConnections.size();}
	SOCKET GetSocket(int index) { return remoteConnections[index].theSocket; }
	void CloseConnectionToAClient(int index);
	void ShutdownServer();
};