#include <string>
#include "UDPServer.h"

//------------------------------------------------------------------------------   
int UDPServer::SendData(int index, const char *msg, int dataSize)//for datagram sockets
{
	unsigned char t;
	for(int i = 0; i < 14; i++)
	{
		t = remoteConnections[index].ip4Data[i];
		remoteConnections[index].remoteInfo.ai_addr->sa_data[i] = t;
	}

	struct sockaddr_in *addr4 = (struct sockaddr_in *) remoteConnections[index].remoteInfo.ai_addr;
	string destIP = inet_ntoa( addr4->sin_addr);
	cout << "sending client " << destIP  <<" : "<<msg <<endl;
		
	return TCPUtils::SendDataUDP(theSocket, msg, dataSize,&remoteConnections[index].remoteInfo);

}
//------------------------------------------------------------------------------   
//useful fro telling people who arent connected stuff...like "server is full" or "you're in" etc
int UDPServer::SendRespnoseData(const char *msg,int dataSize, addrinfo *whomToSend)
{
	return TCPUtils::SendDataUDP(theSocket, msg, dataSize,whomToSend);
}
//------------------------------------------------------------------------------
void UDPServer::GetData(UDPServer::RemoteDataInfo &ret)
{
	
	socklen_t addr_len = sizeof ret.clientInfo;

	ret.numBytes = recvfrom(theSocket, (char *)ret.rawData, MAX_BUFFFER_SIZE-1, 0, (struct sockaddr *)&ret.clientInfo, &addr_len);
	ret.id = -1;
	if (ret.numBytes == SOCKET_ERROR) 
	{
		ReportError("recvfrom()");
		return;
	}
	
	struct sockaddr_in *addr4 = (struct sockaddr_in *)&ret.clientInfo;
	char * clientIP = inet_ntoa(ret.clientInfo.sin_addr);
	struct sockaddr_in * connectionIP = nullptr;
	for(size_t i = 0; i < remoteConnections.size(); i++)
	{
		//check to see if we have this client address stored in our list of clients,if so, which index
		connectionIP = (struct sockaddr_in *)(remoteConnections[i].remoteInfo.ai_addr);

		if(strcmp(inet_ntoa(connectionIP->sin_addr),clientIP) ==0)
		{
			ret.id = i+1;
		}
	}
}
//------------------------------------------------------------------------------


int UDPServer::StartServer(/*int numConnections,*/ char* port)
{
	tv.tv_sec = 0;
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
	//maxConnections = numConnections;
	std::memset(&hints, 0, sizeof(hints)); // zero the rest of the struct 
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE; 				                   
	hints.ai_socktype = SOCK_DGRAM;

	if(getaddrinfo(NULL, port, &hints, &servinfo) != 0)
	{
		ReportError("getaddrinfo()");		// Report the error with our custom function
		Shutdown();				// Shutdown Winsock
		return NETWORK_ERROR;
	}


	//prob shoudl just make my max num sockets now?

	theSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (theSocket == INVALID_SOCKET) 
	{
		ReportError("socket()");		// Report the error with our custom function
		Shutdown();				// Shutdown Winsock
		return NETWORK_ERROR;			// Return an error value
	}

	//nret = bind(remoteConn.theSocket, (LPSOCKADDR)&hints, sizeof(struct sockaddr));
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
		break;
	}



	if (nret == SOCKET_ERROR) 
	{
		ReportError("listener socket: failed to bind() socket\n");
		Shutdown();
		return NETWORK_ERROR;
	}

	//now cerate a sending socket
	waitingForClients = true;
	return NETWORK_OK;
}
//------------------------------------------------------------------------------
//returns the index...mostly used to send client thier info
//int UDPServer::AddClientToList(unsigned char a,unsigned char b,unsigned char c,unsigned char d , int port, int family)
int UDPServer::AddClientToList(addrinfo adder)
{
	RemoteConnection newremote;
	newremote.isActive = true;
	unsigned char t;
	for(int i = 0; i < 14; i++)
	{
		t = adder.ai_addr->sa_data[i];
		newremote.ip4Data[i] = t;
	}

	/*sockaddr_in newClientInfo;
	newClientInfo.sin_addr.S_un.S_un_b.s_b1 = a;
	newClientInfo.sin_addr.S_un.S_un_b.s_b2 = b;
	newClientInfo.sin_addr.S_un.S_un_b.s_b3 = c;
	newClientInfo.sin_addr.S_un.S_un_b.s_b4 = d;


	newremote.remoteInfo.ai_addrlen = sizeof(newClientInfo);
	newremote.remoteInfo.ai_family = family;
	newremote.remoteInfo.ai_socktype = SOCK_DGRAM;
	//newremote.remoteInfo.ai_addr->sa_family
	*/
	newremote.remoteInfo = adder;
	size_t i = 0;
	bool usedOldSpot = false;
	//if we have any unused spaces, lets use that instead of adding more to the array
	for(i = 0; i < remoteConnections.size(); i++)
	{
		if(!remoteConnections[i].isActive)
		{
			remoteConnections[i] = newremote;
			usedOldSpot = true;
			break;
		}
	}
	if(!usedOldSpot)
	{
		remoteConnections.push_back(newremote);
		return remoteConnections.size();
	}

	return (int)i+1;
}
//------------------------------------------------------------------------------
//returns the index...mostly used to send client thier info
bool UDPServer::IsCLientInList(addrinfo newClient)
{
	for(size_t i = 0; i < remoteConnections.size(); i++)
	{
		if(remoteConnections[i].remoteInfo.ai_addr == newClient.ai_addr)
		{
			return true;
		}
	}
	
	return false;
}
//------------------------------------------------------------------------------
//returns the index...mostly used to send client thier info
bool UDPServer::IsCLientInList(int id)
{
	if (id <= 0 || id > remoteConnections.size())
		return false;
	
	if(remoteConnections[id-1].isActive)
		return true;

	return false;
}
//------------------------------------------------------------------------------
//returns the index...mostly used to send client thier info
int UDPServer::GetNumAvailConn()
{
	int numOpenSlots = 0;
	for(size_t i = 0; i < remoteConnections.size(); i++)
	{
		if(!remoteConnections[i].isActive)
			numOpenSlots++;
	}
	
	return numOpenSlots;
}
//------------------------------------------------------------------------------
//returns the index...mostly used to send client thier info
int UDPServer::GetNumActiveUsers()
{
	int numUsers = 0;
	for(size_t i = 0; i < remoteConnections.size(); i++)
	{
		if(remoteConnections[i].isActive)
			numUsers++;
	}
	
	return numUsers;
}


//------------------------------------------------------------------------------
void UDPServer::CloseConnectionToAClient(int id)
{
	//udp sockets dont haev a connecction to close...so, clear the data of the clients entry we had
	if (id <= 0 || id > remoteConnections.size())
		return;

	int index  = id-1;
	remoteConnections[index].isActive = false;
	//freeaddrinfo(&remoteConnections[index].remoteInfo);
}
//------------------------------------------------------------------------------
void UDPServer::ShutdownServer()
{
	freeaddrinfo(servinfo);
	//for (size_t x = 0; x < remoteConnections.size(); x++)
	//	freeaddrinfo(remoteConnections[x].remoteInfo);
	closesocket(theSocket);


	// Shutdown Winsock
	Shutdown();
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
int UDPServer::ServerBroadcast(const char *msg, int dataSize)
{
	int howManySent = 0;
	for (size_t n = 0; n < remoteConnections.size(); n++)
	{
		SendData(n, msg, dataSize);
		howManySent++;
	}

	return howManySent;
}

void UDPServer::PrintCurrentConnectedIPs()
{
	for (size_t i = 0; i < remoteConnections.size(); i++)
	{
		if(remoteConnections[i].isActive)
		{
			struct sockaddr_in *addr4 = (struct sockaddr_in *) remoteConnections[i].remoteInfo.ai_addr;
			string destIP = inet_ntoa( addr4->sin_addr);
			cout << "client " << i+1 << ") " << destIP <<endl;
		}
	}

}