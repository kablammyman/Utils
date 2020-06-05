#pragma once
#include "TCPUtils.h"

class UDPServer : public TCPUtils
{

public:
	struct RemoteConnection
	{
		bool isActive;
		struct addrinfo *remoteInfo;
	};
	struct RemoteDataInfo
	{
		int numBytes;
		char rawData[MAX_STRING_LENGTH];
		int id;
		sockaddr_in clientInfo;
		//struct sockaddr_in clientAdd; 
	};
	int SendData(int index, const char *msg);//for datagram sockets
	int SendRespnoseData(const char *msg,addrinfo *whomToSend);//for datagram sockets
	RemoteDataInfo GetData();//for datagram sockets

	int ServerBroadcast(const char *msg);
	int StartServer(int numConnections, char* port);


	int ChangeToNonBlocking();
	size_t GetNumConnections() {return remoteConnections.size();}

	void CloseConnectionToAClient(int index);
	void ShutdownServer();
	bool AddClientToList(addrinfo *newClient);
private:
	
	bool waitingForClients = false;
	struct addrinfo  *clientAddr; 
	SOCKET theSocket;
	int numCurConnections;
	int maxConnections;
	//we dont need multiple sockets with udp
	//it will lsiten on one socket, and send out messages to all peopel with one sicket
	//but you still need to keep track of the ips we are dealing with
	vector<RemoteConnection> remoteConnections;
};