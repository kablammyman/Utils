
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

//we def should do some array checking, but ill worry about that later
int TCPUtils::ReadIntFromBuffer(unsigned char* buffer, size_t &index)
{
	char intBuffer[4];
	intBuffer[0] = buffer[index];
	intBuffer[1] = buffer[index+1];
	intBuffer[2] = buffer[index+2];
	intBuffer[3] = buffer[index+3];
	int retVal = int((unsigned char)(buffer[0]) << 24 |(unsigned char)(buffer[1]) << 16 |(unsigned char)(buffer[2]) << 8 |(unsigned char)(buffer[3]));
	index += 4;
	return retVal;	
}
float TCPUtils::ReadFloatFromBuffer(unsigned char* buffer, size_t &index)
{
	char floatBuffer[4];
	floatBuffer[0] = buffer[index];
	floatBuffer[1] = buffer[index+1];
	floatBuffer[2] = buffer[index+2];
	floatBuffer[3] = buffer[index+3];

	float x = *(float *)&floatBuffer;
	index +=4;
	return x;
}
char* TCPUtils::ReadHeaderFromBuffer(unsigned char* buffer)
{
	char header[4];
	header[0] = buffer[0];
	header[1] = buffer[1];
	header[2] = buffer[2];
	header[3] = buffer[3];
	return header;
}
char* TCPUtils::ReadStringFromBuffer(unsigned char* buffer, size_t size, size_t &index)
{
	char * retString = new char[size];
	
	for(size_t i = index; i < (index+size); i++)
		retString[i-index] = buffer[i];

	index += size;
	return retString;
}

void TCPUtils::WriteIntToBuffer(int x, unsigned  char* buffer, size_t &index)
{
	buffer[index] = (x >> 24) & 0xFF;
	buffer[index+1] = (x >> 16) & 0xFF;
	buffer[index+2] = (x >> 8) & 0xFF;
	buffer[index+3] = x & 0xFF;
	index += 4;
}
void TCPUtils::WriteFloatToBuffer(float x, unsigned  char* buffer, size_t &index)
{
	int length = sizeof(float);

	for(int i = 0; i < length; i++)
	{
		buffer[index+i] = ((byte*)&x)[i];
	}
	index += 4;
}
void TCPUtils::WriteHeaderToBuffer(unsigned char* header, unsigned char* buffer)
{
	buffer[0] = header[0];
	buffer[1] = header[1];
	buffer[2] = header[2];
	buffer[3] = header[3];
}
void TCPUtils::WriteStringToBuffer(unsigned char* stringInput, unsigned char* buffer, size_t size, size_t &index)
{
	for(size_t i = index; i < (index+size); i++)
		buffer[index+i] = stringInput[i];

	index += size;
}

/*std::string getServerInfo()
{
	int length =sizeof(struct sockaddr);
	int otherCompInfo = getpeername(theSocket,(LPSOCKADDR)&hostInfo,&length);
	return 
} */


///////////////////////
//part of start server code that worked, but unnesc
