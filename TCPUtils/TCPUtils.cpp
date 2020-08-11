#include <string.h> //memset,strlen
#include <string>
#include "TCPUtils.h"

bool TCPUtils::verboseOutput = true;

void TCPUtils::ReportError(std::string  whichFunc)
{
	int errorCode = 0;
#ifdef _WIN32
	errorCode =	WSAGetLastError();
#else
	errorCode = errno;
#endif
	string error = ("Call to " + whichFunc + " returned error ");
	std::cout << error << errorCode <<endl;
	//MessageBox(0, errorMsg, "socketIndication", MB_OK);
}
//------------------------------------------------------------------------------
int TCPUtils::FillTheirInfo(addrinfo *who, SOCKET daSocket)
{
	// Fill a SOCKADDR_IN struct with address information of comp trying to conenct to
	socklen_t length = sizeof(struct sockaddr);
	int otherCompInfo = getpeername(daSocket, (sockaddr *)who, &length);

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
		ReportError("SendDataTCP()");
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
		ReportError("GetDataTCP()");
		return NETWORK_ERROR;
	}

	return nret;// nret contains the number of bytes received
}

//------------------------------------------------------------------------------   
int TCPUtils::SendDataUDP(SOCKET daSocket, const char *msg, int dataLen, addrinfo *whomToSend)//for datagram sockets
{
	//this is a problem! we cant just use strlen since we dont always just send strings!
	int nret = sendto(daSocket, msg, dataLen, 0, whomToSend->ai_addr, whomToSend->ai_addrlen);

	if (nret == SOCKET_ERROR) 
	{
		ReportError("SendDataUDP()");
		return NETWORK_ERROR;
	}

	return nret;// nret contains the number of bytes sent

}
//------------------------------------------------------------------------------
int TCPUtils::GetDataUDP(SOCKET daSocket, char *msg)//for datagram sockets
{
	sockaddr_storage whosSendingMeStuff;
	socklen_t addr_len = sizeof whosSendingMeStuff;
	int numBytes = recvfrom(daSocket, msg, MAX_BUFFFER_SIZE-1, 0, (struct sockaddr *)&whosSendingMeStuff, &addr_len);
	
	if (numBytes == SOCKET_ERROR) 
	{
		ReportError("GetDataUDP()");
		return NETWORK_ERROR;
	}
	//printf("listener: got packet from %s\n",inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s));
	return numBytes;
}
//------------------------------------------------------------------------------
void TCPUtils::CloseConnection(SOCKET daSocket)
{
	MyCloseSocket(daSocket);
}
//------------------------------------------------------------------------------   
int TCPUtils::ChangeToIsBlocking(SOCKET daSocket, bool isBlocking)// Change the socket mode on the listening socket from blocking to non-block 
{
	
#ifdef _WIN32
	// If iMode = 0, blocking is enabled; 
	// If iMode != 0, non-blocking mode is enabled.
	unsigned long NonBlock = 1;
	if(isBlocking)
		NonBlock = 0;
    if (ioctlsocket(daSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
		return -1;
	return 0;
#else
	int flags = fcntl(daSocket, F_GETFL, 0);
	if (flags == -1) 
		return -1;
	if(isBlocking)
		flags &= ~O_NONBLOCK;           // set blocking
	else
		flags |= O_NONBLOCK;	// set non-blocking
	if(fcntl(daSocket, F_SETFL, flags) == 0) 
		return 0;

	return -1;
#endif
}
//------------------------------------------------------------------------------
bool TCPUtils::HasRecivedData(SOCKET daSocket)
{
	FD_ZERO(&read_fds);
	FD_SET(daSocket, &read_fds);

	//windows ignores first param of select, everyone else says The parameter must be set to the highest socket value +1 that is being monitored.
	int result = select(daSocket+1, &read_fds, 0, 0, &tv);
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
	unsigned char intBuffer[4];
	intBuffer[0] = buffer[index];
	intBuffer[1] = buffer[index+1];
	intBuffer[2] = buffer[index+2];
	intBuffer[3] = buffer[index+3];
	int retVal = int((unsigned char)(intBuffer[0]) << 24 |(unsigned char)(intBuffer[1]) << 16 |(unsigned char)(intBuffer[2]) << 8 |(unsigned char)(intBuffer[3]));
	index += 4;
	return retVal;	
}
float TCPUtils::ReadFloatFromBuffer(unsigned char* buffer, size_t &index)
{
	unsigned char floatBuffer[4];
	floatBuffer[0] = buffer[index];
	floatBuffer[1] = buffer[index+1];
	floatBuffer[2] = buffer[index+2];
	floatBuffer[3] = buffer[index+3];

	float x = *(float *)&floatBuffer;
	index +=4;
	return x;
}
void TCPUtils::ReadHeaderFromBuffer(unsigned char* buffer, unsigned char header[5])
{
	header[0] = buffer[0];
	header[1] = buffer[1];
	header[2] = buffer[2];
	header[3] = buffer[3];
	header[4] = '\0';
}
unsigned char* TCPUtils::ReadStringFromBuffer(unsigned char* buffer, size_t size, size_t &index)
{
	unsigned char * retString = new unsigned char[size];
	
	for(size_t i = index; i < (index+size); i++)
		retString[i-index] = buffer[i];

	index += size;
	return retString;
}

void TCPUtils::WriteIntToBuffer(int x,  unsigned char* buffer, size_t &index)
{
	buffer[index] = (x >> 24) & 0xFF;
	buffer[index+1] = (x >> 16) & 0xFF;
	buffer[index+2] = (x >> 8) & 0xFF;
	buffer[index+3] = x & 0xFF;
	index += 4;
}
void TCPUtils::WriteFloatToBuffer(float x, unsigned  char* buffer, size_t &index)
{
	/*int length = sizeof(float);

	for(int i = 0; i < length; i++)
	{
		buffer[index+i] = ((unsigned char*)&x)[i];
	}*/
	buffer[index] = ((unsigned char*)&x)[0];
	buffer[index+1] = ((unsigned char*)&x)[1];
	buffer[index+2] = ((unsigned char*)&x)[2];
	buffer[index+3] = ((unsigned char*)&x)[3];
	index += 4;
}
void TCPUtils::WriteHeaderToBuffer(unsigned char* header, unsigned char* buffer)
{
	buffer[0] = header[0];
	buffer[1] = header[1];
	buffer[2] = header[2];
	buffer[3] = header[3];
}
void TCPUtils::WriteStringToBuffer(char* stringInput, unsigned char* buffer, size_t size, size_t &index)
{
	for(size_t i = index; i < (index+size); i++)
		buffer[index+i] = stringInput[i];

	index += size;
}

void TCPUtils::Shutdown()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

//dellocate the resorces for a socket for windows or *nix
void TCPUtils::MyCloseSocket(SOCKET daSocket)
{
#ifdef _WIN32
	closesocket(daSocket);
#else
	close(daSocket);
#endif
}
/*std::string getServerInfo()
{
	int length =sizeof(struct sockaddr);
	int otherCompInfo = getpeername(theSocket,(LPSOCKADDR)&hostInfo,&length);
	return 
} */
void TCPUtils::print(string output)
{
	if(verboseOutput)
	{
#ifdef __ANDROID_API__
		__android_log_write(ANDROID_LOG_DEBUG, "NetUtils->", output.c_str());
#else
		cout << output <<endl;
#endif
	}
}

///////////////////////
//part of start server code that worked, but unnesc
