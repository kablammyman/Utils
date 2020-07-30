#pragma once
#include "TCPUtils.h"

class TCPServer : public TCPUtils
{
	public:
		struct RemoteComputerConnection
		{
			SOCKET theSocket;
			int portNumber = 0;
			string ipAddy;
			struct addrinfo *remoteInfo;
			//if needed, we can set an id for who or what this is connected to
			int id = 0;
		};
	private:

	bool waitingForClients = false;
	SOCKET listeningSocket;
	int numListeningConnections;
	vector<RemoteComputerConnection> remoteConnections;
	
public:
	

	int SendData(size_t socketIndex, const char *msg);//for stream sockets
	int GetData(size_t socketIndex, char *msg, int dataSize);//for stream sockets

	int ServerBroadcast(const char *msg);
	int HasRecivedData();
	
	
	int StartServer(int numConnections, char* port);
	
	int WaitForFirstClientConnect();
	int WaitForClientAsync();
	//for stream sockets
	
	
	size_t GetNumConnections() {return remoteConnections.size();}
	SOCKET GetSocket(int index) { return remoteConnections[index].theSocket; }
	void CloseConnectionToAClient(int index);
	void ShutdownServer();
};