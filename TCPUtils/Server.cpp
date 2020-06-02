#include <string>
#include "Server.h"



#define MAXBUFLEN 256


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

	return TCPUtils::GetDataUDP(remoteConnections[socketIndex].theSocket, msg);
}
//------------------------------------------------------------------------------


int Server::StartServer(int numConnections, char* port, SOCKET_TYPE socketType)
{
	tv.tv_sec = 1;
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
	memset(&myInfo, 0, sizeof(myInfo)); // zero the rest of the struct 
	myInfo.ai_family = AF_INET;
	myInfo.ai_flags = AI_PASSIVE; 				                   
	//myInfo.sin_port = htons(port);		// Convert integer to network-byte order and insert into the port field		

	if (socketType == STREAM_SOCKET)
		myInfo.ai_socktype = SOCK_STREAM;
	else
		myInfo.ai_socktype = SOCK_DGRAM;

	if(getaddrinfo(NULL, port, &myInfo, &servinfo) != 0)
	{
		ReportError(WSAGetLastError(), "getaddrinfo()");		// Report the error with our custom function
		WSACleanup();				// Shutdown Winsock
		return NETWORK_ERROR;
	}


	if (socketType == STREAM_SOCKET)
	{
		listeningSocket = socket(PF_INET, SOCK_STREAM, 0);
		if (listeningSocket == INVALID_SOCKET) 
		{
			ReportError(WSAGetLastError(), "socket()");		// Report the error with our custom function
			WSACleanup();				// Shutdown Winsock
			return NETWORK_ERROR;			// Return an error value
		}
		
		nret = bind(listeningSocket, (LPSOCKADDR)&myInfo, sizeof(struct sockaddr));
		setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message
	}
	else
	{
		remoteConn.theSocket = socket(PF_INET, SOCK_DGRAM, 0);
		if (remoteConn.theSocket == INVALID_SOCKET) 
		{
			ReportError(WSAGetLastError(), "socket()");		// Report the error with our custom function
			WSACleanup();				// Shutdown Winsock
			return NETWORK_ERROR;			// Return an error value
		}
		
		//nret = bind(remoteConn.theSocket, (LPSOCKADDR)&myInfo, sizeof(struct sockaddr));
		//setsockopt(remoteConn.theSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message
		for(remoteConn.remoteInfo = servinfo; remoteConn.remoteInfo != NULL; remoteConn.remoteInfo = remoteConn.remoteInfo->ai_next) 
		{
			remoteConn.theSocket = socket(remoteConn.remoteInfo->ai_family, SOCK_DGRAM, remoteConn.remoteInfo->ai_protocol);
			if (remoteConn.theSocket == -1) 
			{
				//perror("listener: socket");
				continue;
			}
			nret = bind(remoteConn.theSocket, remoteConn.remoteInfo->ai_addr, remoteConn.remoteInfo->ai_addrlen);
			if (nret == -1) 
			{
				closesocket(remoteConn.theSocket);
				//perror("listener: bind");
				continue;
			}
			else
				break;
		}
	}



	if (nret == SOCKET_ERROR) 
	{
		ReportError(WSAGetLastError(), "listener socket: failed to bind() socket\n");
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
		//FD_SET(remoteConn.theSocket, &master);
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
	freeaddrinfo(servinfo);
	for (size_t x = 0; x < remoteConnections.size(); x++)
		closesocket(remoteConnections[x].theSocket);
	closesocket(listeningSocket);

	// Shutdown Winsock
	WSACleanup();
}
//------------------------------------------------------------------------------
int Server::WaitForFirstTCPClientConnect()
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
int Server::ChangeToNonBlocking(SOCKET daSocket)// Change the socket mode on the listening socket from blocking to non-block 
{
	ULONG NonBlock = 1;
	if (ioctlsocket(daSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
		return -1;
	return 0;
}
//------------------------------------------------------------------------------   
int Server::WaitForTCPClientAsync()
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

		FillTheirInfo(remoteConn.remoteInfo, remoteConn.theSocket);

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


