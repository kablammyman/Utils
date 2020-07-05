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
		unsigned char rawData[MAX_BUFFFER_SIZE];
		int id;
		sockaddr_in clientInfo;
		//struct sockaddr_in clientAdd; 
	};
private:
	bool waitingForClients = false;
	struct addrinfo  *clientAddr; 
	SOCKET theSocket;
	int numCurConnections;
	//int maxConnections;//we didnt even do anything with this. let the lib user decide on max connections

	//we dont need multiple sockets with udp
	//it will lsiten on one socket, and send out messages to all peopel with one sicket
	//but you still need to keep track of the ips we are dealing with
	vector<RemoteConnection> remoteConnections;

public:
	int SendData(int index, const char *msg, int dataSize);//for datagram sockets
	int SendRespnoseData(const char *msg, int dataSize,addrinfo *whomToSend);//for datagram sockets
	void GetData(RemoteDataInfo &ret);//for datagram sockets

	int ServerBroadcast(const char *msg, int dataSize);
	int StartServer(/*int numConnections,*/ char* port);


	int ChangeToNonBlocking();
	size_t GetNumConnections() {return remoteConnections.size();}

	void CloseConnectionToAClient(int index);
	void ShutdownServer();
	int AddClientToList(addrinfo *newClient);
	bool IsCLientInList(addrinfo *newClient);
	bool IsCLientInList(int id);
	int GetNumActiveUsers();
	int GetNumAvailConn();
	bool HasRecivedData(){ return TCPUtils::HasRecivedData(theSocket);  }
};