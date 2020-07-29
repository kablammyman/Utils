#include <string>
#include "TCPServer.h"



//------------------------------------------------------------------------------ 
int TCPServer::SendData(int socketIndex, const char *msg)//for stream sockets
{
	if(socketIndex >= remoteConnections.size())
		return NETWORK_ERROR;

	return TCPUtils::SendDataTCP(remoteConnections[socketIndex].theSocket, msg);
}

//------------------------------------------------------------------------------
int TCPServer::GetData(int socketIndex, char *msg, int dataSize)//for stream sockets
{
	if (socketIndex >= remoteConnections.size())
		return NETWORK_ERROR;

	return TCPUtils::GetDataTCP(remoteConnections[socketIndex].theSocket,msg,dataSize);
}
//------------------------------------------------------------------------------


int TCPServer::StartServer(int numConnections, char* port)
{
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	RemoteComputerConnection remoteConn;
#ifdef _WIN32
	sockVersion = MAKEWORD(2, 2);			// We'd like Winsock version 1.1
	WSAStartup(sockVersion, &wsaData);
#endif
	FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&read_fds);
	int yes = 1;
	int nret = SOCKET_ERROR;

	numListeningConnections = numConnections;
	std::memset(&hints, 0, sizeof(hints)); // zero the rest of the struct 
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE; 				                   
	hints.ai_socktype = SOCK_STREAM;
	

	if(getaddrinfo(NULL, port, &hints, &servinfo) != 0)
	{
		ReportError("getaddrinfo()");		// Report the error with our custom function
		Shutdown();				// Shutdown Winsock
		return NETWORK_ERROR;
	}


	listeningSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (listeningSocket == INVALID_SOCKET) 
	{
		ReportError("socket()");		// Report the error with our custom function
		Shutdown();				// Shutdown Winsock
		return NETWORK_ERROR;			// Return an error value
	}
		
	nret = bind(listeningSocket, (sockaddr *)&hints, sizeof(struct sockaddr));
	setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message

	
	if (nret == SOCKET_ERROR) 
	{
		ReportError("listener socket: failed to bind() socket\n");
		Shutdown();
		return NETWORK_ERROR;
	}


	FD_SET(listeningSocket, &master);
	nret = listen(listeningSocket, numListeningConnections);// Up to 10 connections may wait at any one time to be accept()'ed

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
	TCPUtils::CloseConnection(remoteConnections[index].theSocket);
	// erase the 6th element: myvector.erase(myvector.begin() + 5);
	remoteConnections.erase(remoteConnections.begin()+index);
}
//------------------------------------------------------------------------------
void TCPServer::ShutdownServer()
{
	freeaddrinfo(servinfo);
	for (size_t x = 0; x < remoteConnections.size(); x++)
		closesocket(remoteConnections[x].theSocket);
	closesocket(listeningSocket);

	// Shutdown Winsock
	Shutdown();
}
//------------------------------------------------------------------------------
int TCPServer::WaitForFirstClientConnect()
{
	int yes = 1;

	RemoteComputerConnection remoteConn;
	remoteConn.theSocket = accept(listeningSocket, 0, 0);

	setsockopt(remoteConn.theSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes,sizeof(int)); // lose the pesky "address already in use" error message

	if (remoteConn.theSocket == INVALID_SOCKET)
	{
		ReportError("accept()");
		Shutdown();
		return NETWORK_ERROR;
	}

	FillTheirInfo(remoteConn.remoteInfo, remoteConn.theSocket);

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
		RemoteComputerConnection remoteConn;
		int yes = 1;
		remoteConn.theSocket = accept(listeningSocket, 0, 0);
		setsockopt(remoteConn.theSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message

		if (remoteConn.theSocket == INVALID_SOCKET)
		{
			ReportError("accept()");
			Shutdown();
			return NETWORK_ERROR;
		}

		FillTheirInfo(remoteConn.remoteInfo, remoteConn.theSocket);

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
		int nret = send(remoteConnections[n].theSocket, msg, (int)strlen(msg), 0);
		if (nret != -1)
			howManySent++;
	}

	return howManySent;
}
//------------------------------------------------------------------------------
int TCPServer::HasRecivedData()
{
	for( size_t i =0; i < remoteConnections.size(); i++)
		if(TCPUtils::HasRecivedData(remoteConnections[i].theSocket))
			return i;
	return -1;
}