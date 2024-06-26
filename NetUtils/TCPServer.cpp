#include <string>
#include "TCPServer.h"
#include <string.h> //memset,strlen


//------------------------------------------------------------------------------ 
int TCPServer::SendData(size_t socketIndex, const char *msg)//for stream sockets
{
	if(socketIndex >= remoteConnections.size())
		return NETWORK_ERROR;

	return NetUtils::SendDataTCP(remoteConnections[socketIndex]->theSocket, msg);
}

//------------------------------------------------------------------------------
int TCPServer::GetData(size_t socketIndex, unsigned char *msg, int dataSize)//for stream sockets
{
	if (socketIndex >= remoteConnections.size())
		return NETWORK_ERROR;

	return NetUtils::GetDataTCP(remoteConnections[socketIndex]->theSocket,msg,dataSize);
}
//------------------------------------------------------------------------------
int TCPServer::StartServer(/*int numConnections,*/ const char* port)
{
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	RemoteComputerConnection *remoteConn = new RemoteComputerConnection();
#ifdef _WIN32
	sockVersion = MAKEWORD(2, 2);			// We'd like Winsock version 1.1
	WSAStartup(sockVersion, &wsaData);
#endif
	FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&read_fds);
	int yes = 1;
	int nret = SOCKET_ERROR;

	//numListeningConnections = numConnections;
	memset(&hints, 0, sizeof(hints)); // zero the rest of the struct 
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE; 				                   
	hints.ai_socktype = SOCK_STREAM;
	

	if(getaddrinfo(NULL, port, &hints, &servinfo) != 0)
	{
		ReportError("getaddrinfo()");		// Report the error with our custom function
		Shutdown();				// Shutdown Winsock
		return NETWORK_ERROR;
	}


	listeningSocket = socket(servinfo->ai_family, servinfo->ai_socktype, 0);
	if (listeningSocket == INVALID_SOCKET) 
	{
		ReportError("listeningSocket creation");		// Report the error with our custom function
		Shutdown();				// Shutdown Winsock
		return NETWORK_ERROR;			// Return an error value
	}
#ifndef __ANDROID__	
	//android version doesnt return an int
	nret = bind(listeningSocket, servinfo->ai_addr, (int)servinfo->ai_addrlen);
#else
	bind(listeningSocket, (sockaddr *)&hints, sizeof(struct sockaddr));
#endif
	if (nret == SOCKET_ERROR)
	{
		ReportError("listener socket: failed to bind() socket\n");
		Shutdown();
		return NETWORK_ERROR;
	}

	//freeaddrinfo(servinfo);

	setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message

	FD_SET(listeningSocket, &master);
	nret = listen(listeningSocket, SOMAXCONN);

	if (nret == SOCKET_ERROR) 
	{
		ReportError("listen()");
		Shutdown();
		return NETWORK_ERROR;
	}

	
	waitingForClients = true;
	return NETWORK_OK;
}
//------------------------------------------------------------------------------
void TCPServer::CloseConnectionToAClient(int index)
{
	NetUtils::CloseConnection(remoteConnections[index]->theSocket);
	// erase the 6th element: myvector.erase(myvector.begin() + 5);
	remoteConnections.erase(remoteConnections.begin()+index);
}
//------------------------------------------------------------------------------
void TCPServer::ShutdownServer()
{
	freeaddrinfo(servinfo);
	for (size_t x = 0; x < remoteConnections.size(); x++)
		MyCloseSocket(remoteConnections[x]->theSocket);
	MyCloseSocket(listeningSocket);

	// Shutdown Winsock
	Shutdown();
}
//------------------------------------------------------------------------------
//this is a bloacking method that should run only once!
int TCPServer::WaitForFirstClientConnect()
{
	int yes = 1;

	RemoteComputerConnection *remoteConn = new RemoteComputerConnection();
	remoteConn->theSocket = accept(listeningSocket, 0, 0);

	setsockopt(remoteConn->theSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes,sizeof(int)); // lose the pesky "address already in use" error message

	if (remoteConn->theSocket == INVALID_SOCKET)
	{
		ReportError("accept()");
		Shutdown();
		return NETWORK_ERROR;
	}

	FillTheirInfo(remoteConn->remoteInfo, remoteConn->theSocket);

	remoteConnections.push_back(remoteConn);
	
	/*clientConnection = INVALID_SOCKET;
	while (clientConnection == INVALID_SOCKET)
	{
		clientConnection = accept(listeningSocket, 0, 0);
		//clientConnection = accept(listeningSocket,  (struct sockaddr *)&theirInfo ,&sin_size);
	}
	clientConnection = listeningSocket; 

	int sin_size = sizeof(struct sockaddr_in);
	//Address of a sockaddr structure...Address of a variable containing size of sockaddr struct
	clientConnection = accept(listeningSocket,  (struct sockaddr *)&theirInfo ,&sin_size);*/
	
	return NETWORK_OK;
}
//------------------------------------------------------------------------------
//this should run once a loop
int TCPServer::WaitForClientAsync()
{
	// See if connection pending
	fd_set readSet;
	FD_ZERO(&readSet);
	FD_SET(listeningSocket, &readSet);
	timeval timeout;
	timeout.tv_sec = 0;  // Zero timeout (poll)
	timeout.tv_usec = 0;


	if (select(listeningSocket, &readSet, 0, 0, &timeout) == 1)
	{
		RemoteComputerConnection *remoteConn = new RemoteComputerConnection();
		int yes = 1;
		remoteConn->theSocket = accept(listeningSocket, 0, 0);
		setsockopt(remoteConn->theSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message

		if (remoteConn->theSocket == INVALID_SOCKET)
		{
			ReportError("accept()");
			Shutdown();
			return NETWORK_ERROR;
		}

		FillTheirInfo(remoteConn->remoteInfo, remoteConn->theSocket);

		remoteConnections.push_back(remoteConn);

		return GOT_CONNECTION;
	}
	return NETWORK_OK;
}
//------------------------------------------------------------------------------ 
int TCPServer::ServerBroadcast(const char *msg)//for stream sockets
{
	int howManySent = 0;
	for (size_t n = 0; n < remoteConnections.size(); n++)
	{
		int nret = send(remoteConnections[n]->theSocket, msg, (int)strlen(msg), 0);
		if (nret != -1)
			howManySent++;
	}

	return howManySent;
}
//------------------------------------------------------------------------------
int TCPServer::HasRecivedData()
{
	for( size_t i =0; i < remoteConnections.size(); i++)
		if(NetUtils::HasRecivedData(remoteConnections[i]->theSocket))
			return i;
	return -1;
}