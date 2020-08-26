#include "Client.h"
#include <string.h> //memset,strlen



int Client::ConnectToServer(const char* ip, const char* serverPort,/*const char* listenPort,*/ SOCKET_TYPE socketType)
{
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	int nret;
#ifdef _WIN32
	sockVersion = MAKEWORD(2, 2);
	WSAStartup(sockVersion, &wsaData);
#endif
	print("Client::ConnectToServer about to fd zero\n");
	FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&read_fds);
	int yes = 1;
	print("Client::ConnectToServer fd zero done, about to memset\n");
	// Fill a SOCKADDR_IN struct with address information of host trying to conenct to
	memset(&hints, 0, sizeof(hints)); // zero the rest of the struct 
	print("Client::ConnectToServer amemset done get adder info\n");
	hints.ai_family = AF_INET;
	if (socketType == STREAM_SOCKET)
		hints.ai_socktype = SOCK_STREAM;
	else
		hints.ai_socktype = SOCK_DGRAM;
	
	nret = getaddrinfo(ip, serverPort, &hints, &servinfo);
	if (nret!= 0) 
	{
		//fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		ReportError("ConnectToServer ->getaddrinfo server port()");
		Shutdown();
		return NETWORK_ERROR;
	}
	print("Client::ConnectToServeradder info done, about to set up socket\n");	
	/*nret = getaddrinfo(ip, listenPort, &hints, &listenInfo);
	if (nret!= 0) 
	{
		//fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		ReportError("ConnectToServer ->getaddrinfo listen port()");
		Shutdown();
		return NETWORK_ERROR;
	}*/

	if (socketType == STREAM_SOCKET)
	{
		serverConnection.sendSocket = socket(PF_INET, SOCK_STREAM, 0);
		setsockopt(serverConnection.sendSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message
	}
	else
	{
		//serverConnection.sendSocket = socket(PF_INET, SOCK_DGRAM, 0);
		//setsockopt(serverConnection.sendSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message
		// loop through all the results and make a socket

		//find send socket
		for(serverConnection.remoteInfo = servinfo; serverConnection.remoteInfo != NULL; serverConnection.remoteInfo = serverConnection.remoteInfo->ai_next) 
		{
			serverConnection.sendSocket = socket(serverConnection.remoteInfo->ai_family, SOCK_DGRAM,serverConnection.remoteInfo ->ai_protocol);
			if (serverConnection.sendSocket == -1) 
			{
				//perror("talker: socket");
				continue;
			}
			break;
		}
		
	}
	print("Client::ConnectToServer sup up sockeet done\n");
	if (serverConnection.sendSocket == INVALID_SOCKET)
	{
		ReportError("socket()");
		Shutdown();
		return NETWORK_ERROR;
	}


	if (socketType == STREAM_SOCKET)//if we use this with datagram sokcets, we dont need to senttoand recvFrom...we use send and recv
	{
		FD_SET(serverConnection.sendSocket, &master);//for use with select()
		nret = connect(serverConnection.sendSocket, (sockaddr *)&serverConnection.remoteInfo, sizeof(struct sockaddr));
		//nret = connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));

		if (nret == SOCKET_ERROR)
		{
			ReportError("connect()");
			Shutdown();
			return NETWORK_ERROR;
		}
	}
	/*else
	{
		//int numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,p->ai_addr, p->ai_addrlen)
		SendDataUDP("let me in");
	}*/
	print("Client::ConnectToServer exiting with an netowrk ok\n");
	// Successfully connected!
	return NETWORK_OK;
}
//------------------------------------------------------------------------------ 
int Client::SendDataTCP(const char *msg)//for stream sockets
{

	return NetUtils::SendDataTCP(serverConnection.sendSocket, msg);
}

//------------------------------------------------------------------------------
int Client::GetDataTCP(char *msg, int dataSize)//for stream sockets
{
	//return NetUtils::GetDataTCP(serverConnection.recvSocket,msg,dataSize);
	return NetUtils::GetDataTCP(serverConnection.sendSocket,msg,dataSize);
}

//------------------------------------------------------------------------------   
int Client::SendDataUDP( const char *msg, int dataSize)//for datagram sockets
{
	return NetUtils::SendDataUDP(serverConnection.sendSocket, msg, dataSize, serverConnection.remoteInfo);

}
//------------------------------------------------------------------------------
int Client::GetDataUDP(char *msg)//for datagram sockets
{
	//return NetUtils::GetDataUDP(serverConnection.recvSocket, msg);
	return NetUtils::GetDataUDP(serverConnection.sendSocket, msg);
}
//------------------------------------------------------------------------------
void Client::DisconnectFromServer()
{
	if (servinfo)
	{
		freeaddrinfo(servinfo);
		servinfo = nullptr;
	}
	NetUtils::CloseConnection(serverConnection.sendSocket);
}
//------------------------------------------------------------------------------
bool Client::HasRecivedData()
{
	//return NetUtils::HasRecivedData(serverConnection.recvSocket);
	return NetUtils::HasRecivedData(serverConnection.sendSocket);
}
//-----------------------
int Client::ChangeToIsBlocking(bool isBlocking)
{
	//return NetUtils::ChangeToNonBlocking(serverConnection.recvSocket);
	return NetUtils::ChangeToIsBlocking(serverConnection.sendSocket,isBlocking);
}
/*std::string getServerInfo()
{
int length =sizeof(struct sockaddr);
int otherCompInfo = getpeername(theSocket,(sockaddr *)&hostInfo,&length);
return 
} */
