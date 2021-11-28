#pragma once

#if _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x501
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>


#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <unistd.h>//close()
#include <sys/select.h> //FD_SET,FD_ZERO
#include <sys/ioctl.h> //icotl()
#include <fcntl.h>



#define SOCKET_ERROR -1
#define SOCKET int
#define INVALID_SOCKET 0

#endif


#include <iostream>
#include <vector>
#include <string>

#define MAXBUFLEN 256
#define DEFAULT_BUFLEN 512
#define NETWORK_ERROR -1
#define NETWORK_OK     0
#define GOT_CONNECTION 1
#define MAX_BUFFFER_SIZE 1400

using namespace std;
/*
change it so that each "connection" (aka server or client) has its own
params including port num

have an array of that class*/

#ifdef __ANDROID_API__
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "NetUtils", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "NetUtils", __VA_ARGS__))
#endif




class NetUtils
{
protected:
	struct addrinfo *servinfo, *listenInfo;
	struct addrinfo hints;
#ifdef _WIN32
	WORD sockVersion;
	WSADATA wsaData;
#endif

	int sockfd;
	struct timeval tv;

	fd_set master;   // master file descriptor list
	fd_set read_fds; // temp file descriptor list for select()
	
public:
	char recvBuffer[MAX_BUFFFER_SIZE];
	enum SOCKET_TYPE
	{
		DATAGRAM_SOCKET = 0,
		STREAM_SOCKET 
	};

	void ReportError(std::string  whichFunc);
	int FillTheirInfo(addrinfo *who, SOCKET daSocket);
	
	
	
	int SendDataTCP(SOCKET daSocket, const char *msg);
	int GetDataTCP(SOCKET daSocket, char *msg, int dataSize);
	virtual int ChangeToIsBlocking(SOCKET daSocket, bool isBlocking);

	//for datagram sockets
	int SendDataUDP(SOCKET daSocket, const char *msg, int dataLen,addrinfo *whomToSend);
	int GetDataUDP(SOCKET daSocket, char *msg);

	
	bool HasRecivedData(SOCKET daSocket);
	void CloseConnection(SOCKET daSocket);
	void Shutdown();
	void MyCloseSocket(SOCKET daSocket);

	static int ReadIntFromBuffer(unsigned char *buffer, size_t &index);
	static float ReadFloatFromBuffer(unsigned char *buffer, size_t &index);
	static void ReadHeaderFromBuffer(unsigned char *buffer,unsigned char header[5]);
	static unsigned char * ReadStringFromBuffer(unsigned char *buffer,size_t size,size_t &index);

	static void WriteIntToBuffer(int x, unsigned char *buffer, size_t &index);
	static void WriteFloatToBuffer(float x,unsigned char *buffer, size_t &index);
	static void WriteHeaderToBuffer(unsigned char *header,unsigned  char *buffer);
	static void WriteStringToBuffer(char *stringInput, unsigned char *buffer,size_t size,size_t &index);
	
	static bool verboseOutput;
	static void print(string output);
};