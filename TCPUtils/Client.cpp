#include "Client.h"

int Client::ConnectToServer(const char* ip, const char* port, SOCKET_TYPE socketType)
{
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	int nret;
	sockVersion = MAKEWORD(2, 2);
	WSAStartup(sockVersion, &wsaData);
	FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&read_fds);
	int yes = 1;
	
	// Fill a SOCKADDR_IN struct with address information of host trying to conenct to
	memset(&myInfo, 0, sizeof(myInfo)); // zero the rest of the struct 
	myInfo.ai_family = AF_INET;
	if (socketType == STREAM_SOCKET)
		myInfo.ai_socktype = SOCK_STREAM;
	else
		myInfo.ai_socktype = SOCK_DGRAM;
	
	nret = getaddrinfo(ip, port, &myInfo, &servinfo);
	if (nret!= 0) 
	{
		//fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		ReportError(WSAGetLastError(), "socket()");
		WSACleanup();
		return NETWORK_ERROR;
	}
		

	if (socketType == STREAM_SOCKET)
	{
		serverConnection.theSocket = socket(PF_INET, SOCK_STREAM, 0);
		setsockopt(serverConnection.theSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message
	}
	else
	{
		//serverConnection.theSocket = socket(PF_INET, SOCK_DGRAM, 0);
		//setsockopt(serverConnection.theSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message
		// loop through all the results and make a socket

		for(serverConnection.remoteInfo = servinfo; serverConnection.remoteInfo != NULL; serverConnection.remoteInfo = serverConnection.remoteInfo->ai_next) 
		{
			serverConnection.theSocket = socket(serverConnection.remoteInfo->ai_family, SOCK_DGRAM,serverConnection.remoteInfo ->ai_protocol);
			if (serverConnection.theSocket == -1) 
			{
				//perror("talker: socket");
				continue;
			}
			break;
		}
	}

	if (serverConnection.theSocket == INVALID_SOCKET)
	{
		ReportError(WSAGetLastError(), "socket()");
		WSACleanup();
		return NETWORK_ERROR;
	}


	if (socketType == STREAM_SOCKET)//if we use this with datagram sokcets, we dont need to senttoand recvFrom...we use send and recv
	{
		FD_SET(serverConnection.theSocket, &master);//for use with select()
		int nret = connect(serverConnection.theSocket, (LPSOCKADDR)&serverConnection.remoteInfo, sizeof(struct sockaddr));
		//nret = connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));

		if (nret == SOCKET_ERROR)
		{
			ReportError(WSAGetLastError(), "connect()");
			WSACleanup();
			return NETWORK_ERROR;
		}
	}
	else
	{
		//int numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,p->ai_addr, p->ai_addrlen)
		SendDataUDP("let me in");
	}

	// Successfully connected!
	return NETWORK_OK;
}
//------------------------------------------------------------------------------ 
int Client::SendDataTCP(const char *msg)//for stream sockets
{

	return TCPUtils::SendDataTCP(serverConnection.theSocket, msg);
}

//------------------------------------------------------------------------------
int Client::GetDataTCP(char *msg, int dataSize)//for stream sockets
{

	return TCPUtils::GetDataTCP(serverConnection.theSocket,msg,dataSize);
}

//------------------------------------------------------------------------------   
int Client::SendDataUDP( const char *msg)//for datagram sockets
{
	return TCPUtils::SendDataUDP(serverConnection.theSocket, msg, serverConnection.remoteInfo);

}
//------------------------------------------------------------------------------
int Client::GetDataUDP( char *msg)//for datagram sockets
{

	return TCPUtils::GetDataUDP(serverConnection.theSocket, msg);
}
//------------------------------------------------------------------------------
void Client::DisconnectFromServer()
{
	freeaddrinfo(servinfo);
	TCPUtils::CloseConnection(serverConnection.theSocket);
}
//------------------------------------------------------------------------------
bool Client::HasRecivedData()
{
	return TCPUtils::HasRecivedData(serverConnection.theSocket);
}
/*std::string getServerInfo()
{
int length =sizeof(struct sockaddr);
int otherCompInfo = getpeername(theSocket,(LPSOCKADDR)&hostInfo,&length);
return 
} */
