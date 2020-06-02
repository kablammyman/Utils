
#include <string>
#include "TCPUtils.h"


void TCPUtils::ReportError(int errorCode, std::string  whichFunc)
{
	string error = ("Call to " + whichFunc + " returned error ");
	std::cout << error << errorCode <<endl;
	//MessageBox(0, errorMsg, "socketIndication", MB_OK);
}
//------------------------------------------------------------------------------
int TCPUtils::FillTheirInfo(addrinfo *who, SOCKET daSocket)
{
	// Fill a SOCKADDR_IN struct with address information of comp trying to conenct to
	int length = sizeof(struct sockaddr);
	int otherCompInfo = getpeername(daSocket, (LPSOCKADDR)&who, &length);

	return otherCompInfo;
	/*
	htons() -- "Host to Network Short"
	htonl() -- "Host to Network Long"
	ntohs() -- "Network to Host Short"
	ntohl() -- "Network to Host Long"
	*/

	/*who->sin_family;
	who->sin_addr.s_addr;
	who->sin_port;*/
}
//------------------------------------------------------------------------------ 
int TCPUtils::SendDataTCP(SOCKET daSocket, const char *msg)//for stream sockets
{
	int nret = send(daSocket, msg, (int)strlen(msg), 0);

	if (nret == SOCKET_ERROR) 
	{
		ReportError(WSAGetLastError(), "send()");
		return NETWORK_ERROR;
	}

	return nret;// nret contains the number of bytes sent

}

//------------------------------------------------------------------------------
int TCPUtils::GetDataTCP(SOCKET daSocket, char *msg, int dataSize)//for stream sockets
{
	int nret = recv(daSocket, msg, dataSize, 0);// 256 = Complete size of buffer	  

	if (nret == SOCKET_ERROR) 
	{
		ReportError(WSAGetLastError(), "recv()");
		return NETWORK_ERROR;
	}

	return nret;// nret contains the number of bytes received
}

//------------------------------------------------------------------------------   
int TCPUtils::SendDataUDP(SOCKET daSocket, const char *msg, addrinfo *whomToSend)//for datagram sockets
{
	int nret = sendto(daSocket, msg, (int)strlen(msg), 0, whomToSend->ai_addr, whomToSend->ai_addrlen);

	if (nret == SOCKET_ERROR) 
	{
		ReportError(WSAGetLastError(), "send()");
		return NETWORK_ERROR;
	}

	return nret;// nret contains the number of bytes sent

}
//------------------------------------------------------------------------------
int TCPUtils::GetDataUDP(SOCKET daSocket, char *msg)//for datagram sockets
{
	sockaddr_storage whosSendingMeStuff;
	socklen_t addr_len = sizeof whosSendingMeStuff;
	int nret = recvfrom(daSocket, msg, MAX_STRING_LENGTH-1, 0, (struct sockaddr *)&whosSendingMeStuff, &addr_len);

	if (nret == SOCKET_ERROR) 
	{
		ReportError(WSAGetLastError(), "recv()");
		return NETWORK_ERROR;
	}
//	printf("listener: got packet from %s\n",inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s));
	return nret;// nret contains the number of bytes received
}
//------------------------------------------------------------------------------
void TCPUtils::CloseConnection(SOCKET daSocket)
{
	closesocket(daSocket);
}

//------------------------------------------------------------------------------
bool TCPUtils::HasRecivedData(SOCKET daSocket)
{
	FD_ZERO(&read_fds);
	FD_SET(daSocket, &read_fds,0);

	int result = select(0, &read_fds, 0, 0, &tv);
	if (result > 0)
	{
		if (FD_ISSET(daSocket, &read_fds))
		{
			return true;
		}
	}
	return false;
}

/*std::string getServerInfo()
{
	int length =sizeof(struct sockaddr);
	int otherCompInfo = getpeername(theSocket,(LPSOCKADDR)&hostInfo,&length);
	return 
} */


///////////////////////
//part of start server code that worked, but unnesc
