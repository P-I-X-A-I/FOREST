#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>


typedef struct {

	SOCKET receiveSocket;
	SOCKET sendSocket;

	struct sockaddr_in receiveAddr;
	struct sockaddr_in sendAddr;

	int mSec;
	int bufSize = 256;
	int receiveLength = 0;
	char receiveBuffer[256];

	short stripe[24][2];
	float stripeMatrix[24][2][12];
	bool isFirstSetStripe[24];
	bool isFirstSetScene[3];

	int scene_number;
	int current_objINDEX;
	float current_transY;

	bool isTapped;

	int STAGE_MODE;

	HANDLE h_mutex;

} UDP_STRUCT;


class UDPClass
{
public:

	UDP_STRUCT arg_STRUCT;

	SOCKET socket_dsc_send;
	SOCKET socket_dsc_receive;
	struct sockaddr_in addr_send;
	struct sockaddr_in addr_receive;

	HANDLE rThread;
	DWORD dwThreadID;

	UDPClass();
	~UDPClass();

	bool createReceiveSocket( int port );
	bool createSendSocket(const char* IP, int port);
	void sendData(char* buffer, int length );
	char* readReceiveBuffer();

};

