#pragma once
#include "NetUtils.h"

class UDPServer : public NetUtils
{
public:
	struct RemoteConnection
	{
		bool isActive;
		struct addrinfo remoteInfo;
		unsigned char ip4Data[14];
		RemoteConnection()
		{
			ip4Data[0] = '\0';
			isActive = false;
		}
	};
	struct RemoteDataInfo
	{
		int numBytes;
		unsigned char rawData[MAX_BUFFFER_SIZE];
		int id;
		sockaddr_in clientInfo;
		//struct sockaddr_in clientAdd; 
	};
protected:
	bool waitingForClients = false;
	struct addrinfo  *clientAddr; 
	//SOCKET recvSocket;
	//SOCKET sendSocket;
	SOCKET theSocket;
	int numCurConnections;
	//int maxConnections;//we didnt even do anything with this. let the lib user decide on max connections

	//we dont need multiple sockets with udp
	//it will lsiten on one socket, and send out messages to all peopel with one sicket
	//but you still need to keep track of the ips we are dealing with
	vector<RemoteConnection *> remoteConnections;

public:
	int SendData(int index, const char *msg, int dataSize);//for datagram sockets
	int SendRespnoseData(const char *msg, int dataSize,addrinfo *whomToSend);//for datagram sockets
	void GetData(RemoteDataInfo &ret);//for datagram sockets

	int ServerBroadcast(const char *msg, int dataSize);
	int StartServer(/*int numConnections,*/ char* port);


	size_t GetNumConnections() {return remoteConnections.size();}

	void CloseConnectionToAClient(int index);
	void ShutdownServer();
	//int AddClientToList(unsigned char a,unsigned char b,unsigned char c,unsigned char d , int port, int family);
	int AddClientToList(addrinfo adder);
	bool IsCLientInList(addrinfo newClient);
	bool IsCLientInList(size_t id);
	int GetNumActiveUsers();
	int GetNumAvailConn();
	bool HasRecivedData(){ return NetUtils::HasRecivedData(theSocket);  }
	int ChangeToIsBlocking(bool isBlocking) {return NetUtils::ChangeToIsBlocking(theSocket,isBlocking);}
	void PrintCurrentConnectedIPs();
};