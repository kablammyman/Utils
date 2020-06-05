#include <string>
#include "UDPServer.h"

//------------------------------------------------------------------------------   
int UDPServer::SendData(int index, const char *msg)//for datagram sockets
{

	return TCPUtils::SendDataUDP(theSocket, msg, remoteConnections[index].remoteInfo);

}
//------------------------------------------------------------------------------   
//useful fro telling people who arent connected stuff...like "server is full" or "you're in" etc
int UDPServer::SendRespnoseData(const char *msg,addrinfo *whomToSend)
{
	return TCPUtils::SendDataUDP(theSocket, msg, whomToSend);
}
//------------------------------------------------------------------------------
UDPServer::RemoteDataInfo UDPServer::GetData()//for datagram sockets
{
	//return TCPUtils::GetDataUDP(remoteConnections.theSocket, msg);

	UDPServer::RemoteDataInfo ret;
	socklen_t addr_len = sizeof ret.clientInfo;

	ret.numBytes = recvfrom(theSocket, ret.rawData, MAX_STRING_LENGTH-1, 0, (struct sockaddr *)&ret.clientInfo, &addr_len);
	ret.id = -1;
	if (ret.numBytes == SOCKET_ERROR) 
	{
		ReportError(WSAGetLastError(), "recv()");
		sprintf(ret.rawData,"recvfrom error: %s",WSAGetLastError());
		ret.numBytes = NETWORK_ERROR;
		return ret;
	}
	//	printf("listener: got packet from %s\n",inet_ntop(whosSendingMeStuff.ss_family,get_in_addr((struct sockaddr *)&whosSendingMeStuff),s, sizeof s));
	/*
	whosSendingMeStuff.ss_family, //AF_INET
	get_in_addr((struct sockaddr *)&whosSendingMeStuff)// the ip address
	,s, //where to place teh string
	sizeof s)//te size of the string
	*/
	//addrinfo *remote;
	//remote->ai_addr = (struct sockaddr *)&whosSendingMeStuff;
	//remote->ai_addrlen = sizeof(whosSendingMeStuff);

	struct sockaddr_in servaddr;
	//ill clean this up later
	for(size_t i = 0; i < remoteConnections.size(); i++)
	{
		//check to see if we have this client address stored in our list of clients,if so, which index
		if(remoteConnections[i].remoteInfo->ai_addr == (struct sockaddr *)&ret.clientInfo)
		{
			//this is a connected person sending us something
			cout << "got something from a connected person\n";
			ret.id = i;
		}
	}
	return ret;// nret contains the number of bytes received
}
//------------------------------------------------------------------------------


int UDPServer::StartServer(int numConnections, char* port)
{
	tv.tv_sec = 1;
	tv.tv_usec = 0;

#ifdef _WIN32
	sockVersion = MAKEWORD(2, 2);			// We'd like Winsock version 1.1
	WSAStartup(sockVersion, &wsaData);
#endif
	FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&read_fds);
	int yes = 1;
	int nret = SOCKET_ERROR;
	
	numCurConnections = 0;
	maxConnections = numConnections;
	memset(&myInfo, 0, sizeof(myInfo)); // zero the rest of the struct 
	myInfo.ai_family = AF_INET;
	myInfo.ai_flags = AI_PASSIVE; 				                   
	//myInfo.sin_port = htons(port);		// Convert integer to network-byte order and insert into the port field		
	myInfo.ai_socktype = SOCK_DGRAM;

	if(getaddrinfo(NULL, port, &myInfo, &servinfo) != 0)
	{
		ReportError(WSAGetLastError(), "getaddrinfo()");		// Report the error with our custom function
		WSACleanup();				// Shutdown Winsock
		return NETWORK_ERROR;
	}


	//prob shoudl just make my max num sockets now?

	theSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (theSocket == INVALID_SOCKET) 
	{
		ReportError(WSAGetLastError(), "socket()");		// Report the error with our custom function
		WSACleanup();				// Shutdown Winsock
		return NETWORK_ERROR;			// Return an error value
	}

	//nret = bind(remoteConn.theSocket, (LPSOCKADDR)&myInfo, sizeof(struct sockaddr));
	//setsockopt(remoteConn.theSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message
	for(clientAddr = servinfo; clientAddr != NULL; clientAddr = clientAddr->ai_next) 
	{
		theSocket = socket(clientAddr->ai_family, SOCK_DGRAM, clientAddr->ai_protocol);
		if (theSocket == -1) 
		{
			//perror("listener: socket");
			continue;
		}
		nret = bind(theSocket, clientAddr->ai_addr, clientAddr->ai_addrlen);
		if (nret == -1) 
		{
			closesocket(theSocket);
			//perror("listener: bind");
			continue;
		}
		else
			break;
	}



	if (nret == SOCKET_ERROR) 
	{
		ReportError(WSAGetLastError(), "listener socket: failed to bind() socket\n");
		WSACleanup();
		return NETWORK_ERROR;
	}

	waitingForClients = true;
	return NETWORK_OK;
}
//------------------------------------------------------------------------------
bool UDPServer::AddClientToList(addrinfo *newClient)
{
	RemoteConnection newremote;
	newremote.isActive = true;
	newremote.remoteInfo = newClient;
	
	remoteConnections.push_back(newremote);
	//later check to see if this person already exists itnhe lsit

	return true;
}
//------------------------------------------------------------------------------
void UDPServer::CloseConnectionToAClient(int index)
{
	//udp sockets dont haev a connecction to close...so we cna start to ignore thier packets i guwes
	remoteConnections[index].isActive = false;
}
//------------------------------------------------------------------------------
void UDPServer::ShutdownServer()
{
	freeaddrinfo(servinfo);
	for (size_t x = 0; x < remoteConnections.size(); x++)
		freeaddrinfo(remoteConnections[x].remoteInfo);
	closesocket(theSocket);

	// Shutdown Winsock
	WSACleanup();
}
//------------------------------------------------------------------------------   
int UDPServer::ChangeToNonBlocking()// Change the socket mode on the listening socket from blocking to non-block 
{
	ULONG NonBlock = 1;
	if (ioctlsocket(theSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
		return -1;
	return 0;
}
//------------------------------------------------------------------------------   
int UDPServer::ServerBroadcast(const char *msg)
{
	int howManySent = 0;
	for (size_t n = 0; n < remoteConnections.size(); n++)
	{
		SendData(n, msg);
		howManySent++;
	}

	return howManySent;
}
