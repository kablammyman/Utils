#include "Client.h"

int Client::ConnectToServer(string ip, int port, SOCKET_TYPE socketType)
{
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	sockVersion = MAKEWORD(1, 1);
	WSAStartup(sockVersion, &wsaData);
	FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&read_fds);
	int yes = 1;


	serverConnection.portNumber = port;
	serverConnection.ipAddy = ip;
	if (socketType == STREAM_SOCKET)
	{
		serverConnection.theSocket = socket(PF_INET, SOCK_STREAM, 0);
		setsockopt(serverConnection.theSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message
	}
	else
	{
		serverConnection.theSocket = socket(PF_INET, SOCK_DGRAM, 0);
		setsockopt(serverConnection.theSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // lose the pesky "address already in use" error message
	}

	if (serverConnection.theSocket == INVALID_SOCKET)
	{
		ReportError(WSAGetLastError(), "socket()");
		WSACleanup();
		return NETWORK_ERROR;
	}

	// Fill a SOCKADDR_IN struct with address information of host trying to conenct to
	serverConnection.remoteInfo.sin_family = AF_INET;
	serverConnection.remoteInfo.sin_addr.s_addr = inet_addr(serverConnection.ipAddy.c_str());
	serverConnection.remoteInfo.sin_port = htons(serverConnection.portNumber);// Change to network-byte order and insert into port field THIS HAS TO BE SET TO WHAT THE SERVER PORT IS LISTENING ON FOR DATAGRAM    
	memset(&(serverConnection.remoteInfo.sin_zero), '\0', 8); // zero the rest of the struct

	FD_SET(serverConnection.theSocket, &master);//for use with select()

										  // Connect to the server
	if (socketType == STREAM_SOCKET)//if we use this with datagram sokcets, we dont need to senttoand recvFrom...we use send and recv
	{
		int nret = connect(serverConnection.theSocket, (LPSOCKADDR)&serverConnection.remoteInfo, sizeof(struct sockaddr));
		//nret = connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));

		if (nret == SOCKET_ERROR)
		{
			ReportError(WSAGetLastError(), "connect()");
			WSACleanup();
			return NETWORK_ERROR;
		}
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

	return TCPUtils::GetDataUDP(serverConnection.theSocket, msg, serverConnection.remoteInfo);
}
//------------------------------------------------------------------------------
void Client::DisconnectFromServer()
{
	TCPUtils::CloseConnection(serverConnection.theSocket);
}
//------------------------------------------------------------------------------
bool Client::HasRecivedData()
{
	return TCPUtils::HasRecivedData(serverConnection.theSocket);
}
