#include <windows.h>
#include <string>
#include "Server.h"



//------------------------------------------------------------------------------ 
int Server::SendDataTCP(int socketIndex, const char *msg)//for stream sockets
{
	if(socketIndex >= remoteConnections.size())
		return NETWORK_ERROR;

	return TCPUtils::SendDataTCP(remoteConnections[socketIndex].theSocket, msg);
}

//------------------------------------------------------------------------------
int Server::GetDataTCP(int socketIndex, char *msg, int dataSize)//for stream sockets
{
	if (socketIndex >= remoteConnections.size())
		return NETWORK_ERROR;

	return TCPUtils::GetDataTCP(remoteConnections[socketIndex].theSocket,msg,dataSize);
}

//------------------------------------------------------------------------------   
int Server::SendDataUDP(int socketIndex, const char *msg)//for datagram sockets
{
	if (socketIndex >= remoteConnections.size())
		return NETWORK_ERROR;
	
	return TCPUtils::SendDataUDP(remoteConnections[socketIndex].theSocket, msg, remoteConnections[socketIndex].remoteInfo);

}
//------------------------------------------------------------------------------
int Server::GetDataUDP(int socketIndex, char *msg)//for datagram sockets
{
	if (socketIndex >= remoteConnections.size())
		return NETWORK_ERROR;

	return TCPUtils::GetDataUDP(remoteConnections[socketIndex].theSocket, msg, remoteConnections[socketIndex].remoteInfo);
}
//------------------------------------------------------------------------------
int Server::StartServer(int numConnections, int port, SOCKET_TYPE socketType)
{
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	RemoteComputerConnection remoteConn;
	sockVersion = MAKEWORD(1, 1);			// We'd like Winsock version 1.1
	WSAStartup(sockVersion, &wsaData);
	FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&read_fds);
	int yes = 1;
	int nret;

	numListeningConnections = numConnections;

	if (socketType == STREAM_SOCKET)
	{
		listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
		setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message
	}
	else
	{
		remoteConn.theSocket = socket(AF_INET, SOCK_DGRAM, 0);
		setsockopt(remoteConn.theSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message
	}

	if (listeningSocket == INVALID_SOCKET) 
	{
		ReportError(WSAGetLastError(), "socket()");		// Report the error with our custom function
		WSACleanup();				// Shutdown Winsock
		return NETWORK_ERROR;			// Return an error value
	}

	myInfo.sin_family = AF_INET;
	myInfo.sin_addr.s_addr = INADDR_ANY;	// Since this socket is listening for connections, any local address will do						                   
	myInfo.sin_port = htons(port);		// Convert integer to network-byte order and insert into the port field		
	memset(&(myInfo.sin_zero), '\0', 8); // zero the rest of the struct 

	if (socketType == STREAM_SOCKET)// Bind the socket to our local server address
		nret = bind(listeningSocket, (LPSOCKADDR)&myInfo, sizeof(struct sockaddr));
	else
		nret = bind(remoteConn.theSocket, (LPSOCKADDR)&myInfo, sizeof(struct sockaddr));

	if (nret == SOCKET_ERROR) 
	{
		ReportError(WSAGetLastError(), "bind()");
		WSACleanup();
		return NETWORK_ERROR;
	}

	if (socketType == STREAM_SOCKET)// add the listener to the master set
	{
		FD_SET(listeningSocket, &master);
		nret = listen(listeningSocket, numListeningConnections);// Up to 10 connections may wait at any one time to be accept()'ed

		if (nret == SOCKET_ERROR) 
		{
			ReportError(WSAGetLastError(), "listen()");
			WSACleanup();
			return NETWORK_ERROR;
		}
	}
	else
	{
		FD_SET(remoteConn.theSocket, &master);
		remoteConnections.push_back(remoteConn);
	}

	waitingForClients = true;
	return NETWORK_OK;
}
//------------------------------------------------------------------------------
void Server::CloseConnectionToAClient(int index)
{
	TCPUtils::CloseConnection(remoteConnections[index].theSocket);
	// erase the 6th element: myvector.erase(myvector.begin() + 5);
	remoteConnections.erase(remoteConnections.begin()+index);
}
//------------------------------------------------------------------------------
void Server::ShutdownServer()
{

	for (size_t x = 0; x < remoteConnections.size(); x++)
		closesocket(remoteConnections[x].theSocket);
	closesocket(listeningSocket);

	// Shutdown Winsock
	WSACleanup();
}
//------------------------------------------------------------------------------
int Server::WaitForFirstClientConnect()
{
	int yes = 1;

	RemoteComputerConnection remoteConn;
	remoteConn.theSocket = accept(listeningSocket, 0, 0);

	setsockopt(remoteConn.theSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes,sizeof(int)); // lose the pesky "address already in use" error message

	if (remoteConn.theSocket == INVALID_SOCKET)
	{
		ReportError(WSAGetLastError(), "accept()");
		WSACleanup();
		return NETWORK_ERROR;
	}

	FillTheirInfo(&remoteConn.remoteInfo, remoteConn.theSocket);

	remoteConnections.push_back(remoteConn);
	/*
	clientConnection = INVALID_SOCKET;
	while (clientConnection == INVALID_SOCKET)
	{
	clientConnection = accept(listeningSocket, 0, 0);
	//clientConnection = accept(listeningSocket,  (struct sockaddr *)&theirInfo ,&sin_size);
	}
	clientConnection = listeningSocket; 

	int sin_size = sizeof(struct sockaddr_in);
	//Address of a sockaddr structure...Address of a variable containing size of sockaddr struct
	clientConnection = accept(listeningSocket,  (struct sockaddr *)&theirInfo ,&sin_size);
	*/
	//sprintf(message,"we started a server listening on port %d",portNumber);
	// MessageBox(0, message, "Server message", MB_OK);
	return NETWORK_OK;
}
//------------------------------------------------------------------------------   
int Server::ChangeToNonBlocking(SOCKET daSocket)// Change the socket mode on the listening socket from blocking to non-block 
{
	ULONG NonBlock = 1;
	if (ioctlsocket(daSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
		return -1;
	return 0;
}
//------------------------------------------------------------------------------   
int Server::WaitForClientAsync()
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
			ReportError(WSAGetLastError(), "accept()");
			WSACleanup();
			return NETWORK_ERROR;
		}

		FillTheirInfo(&remoteConn.remoteInfo, remoteConn.theSocket);

		remoteConnections.push_back(remoteConn);

		return GOT_CONNECTION;
	}
	return NETWORK_OK;
}
//------------------------------------------------------------------------------ 
int Server::ServerBroadcast(const char *msg)//for stream sockets
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
int Server::HasRecivedData()
{
	for( size_t i =0; i < remoteConnections.size(); i++)
		if(TCPUtils::HasRecivedData(remoteConnections[i].theSocket))
			return i;
	return -1;
}

/*std::string getServerInfo()
{
int length =sizeof(struct sockaddr);
int otherCompInfo = getpeername(theSocket,(LPSOCKADDR)&hostInfo,&length);
return 
} */


