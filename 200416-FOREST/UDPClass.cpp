#include "stdafx.h"
#include "UDPClass.h"


// function prototype
void decomposeUDPmessage( char* sPtr, UDP_STRUCT* structPtr );


DWORD WINAPI receiveProc(LPVOID arg)
{
	void* tempPtr = (void*)arg;
	UDP_STRUCT* tempSTRUCT = (UDP_STRUCT*)tempPtr;


	char buffer[256];	
	SOCKET rSock = tempSTRUCT->receiveSocket;
	SOCKET sSock = tempSTRUCT->sendSocket;
	struct sockaddr_in receive_addr = tempSTRUCT->receiveAddr;
	struct sockaddr_in send_addr = tempSTRUCT->sendAddr;

	int rLength;
	int sendLength;

	int loop_mSec = tempSTRUCT->mSec;
	int receiveBufSize = tempSTRUCT->bufSize;
	HANDLE mutex = tempSTRUCT->h_mutex;

	if (rSock == NULL)
	{
		printf("receive socket is NULL.... return\n");
		return 0;
	}

	
	while (1)
	{
		// receive something
		memset(buffer, 0, sizeof(buffer));
		rLength = recvfrom(rSock, buffer, sizeof(buffer), 0, NULL, 0);
		//recv(rSock, buffer, sizeof(buffer), 0);


		// copy reveived data to struct buffer
		// lock

		WaitForSingleObject(mutex, 0);
		memset(tempSTRUCT->receiveBuffer, 0, receiveBufSize);
		memcpy(tempSTRUCT->receiveBuffer, buffer, 256);
		tempSTRUCT->receiveLength = rLength;

		// decompose UDP receive message
		decomposeUDPmessage(tempSTRUCT->receiveBuffer, tempSTRUCT);

		ReleaseMutex(mutex);



		Sleep(loop_mSec);
	}

	return 0;
}




void decomposeUDPmessage(char* sPtr, UDP_STRUCT* structPtr )
{
	char kugiri[] = "/";
	char* tok;
	char* ctx;

	tok = strtok_s(sPtr, kugiri, &ctx);

	while (tok != NULL)
	{
		if (strcmp(tok, "scene") == 0)
		{
			// get scene number
			tok = strtok_s(NULL, kugiri, &ctx);
			if (tok == NULL) { break; }

			int sceneNum = atoi(tok);

			if (sceneNum == 0)
			{
			
			}
			else if (sceneNum == 1)// scene 1
			{
				// init all
				if (structPtr->isFirstSetScene[0])
				{
					printf("go to scene 1\n");
					structPtr->scene_number = 1;

					structPtr->isFirstSetScene[1] = true;
					structPtr->isFirstSetScene[2] = true;

					for (int i = 0; i < 24; i++)
					{
						structPtr->isFirstSetStripe[i] = true;
						structPtr->stripe[i][0] = 0;
						structPtr->stripe[i][1] = 0;
					}


					structPtr->isFirstSetScene[0] = false;
				}

				// get stage mode
				tok = strtok_s(NULL, kugiri, &ctx);
				int stageValue = atoi(tok);
				structPtr->STAGE_MODE = stageValue;

			}
			else if (sceneNum == 2)// scene 2
			{
				// get object index
				tok = strtok_s(NULL, kugiri, &ctx);
				if (tok == NULL) { break; }

				int objIndex = atoi(tok);

				// get transY
				tok = strtok_s(NULL, kugiri, &ctx);
				if (tok == NULL) { break; }

				float transY = (float)atof(tok);

				structPtr->current_objINDEX = objIndex;
				structPtr->current_transY = transY;


				if (structPtr->isFirstSetScene[1])
				{
					printf("go to scene 2\n");
					structPtr->scene_number = 2;
					
					structPtr->isFirstSetScene[1] = false;
				}
			}
			else if (sceneNum == 3)// scene 3
			{


				if (structPtr->isFirstSetScene[2])
				{
					printf("go to scene 3\n");

					structPtr->scene_number = 3;

					structPtr->isFirstSetScene[0] = true;


					structPtr->isFirstSetScene[2] = false;
				}
			}

		}
		else if (strcmp(tok, "stripe") == 0)
		{
			// get index
			tok = strtok_s(NULL, kugiri, &ctx);
			if (tok == NULL) { break; }

			int INDEX = atoi(tok);

			// get value
			tok = strtok_s(NULL, kugiri, &ctx);
			if (tok == NULL) { break; }

			short stripeValue1 = (short)atoi(tok);

			tok = strtok_s(NULL, kugiri, &ctx);
			if (tok == NULL) { break; }

			short stripeValue2 = (short)atoi(tok);


			// set stripe value
			if (structPtr->isFirstSetStripe[INDEX])
			{
				printf("stripe[%d] : %d : %d\n", INDEX, stripeValue1, stripeValue2);

				structPtr->stripe[INDEX][0] = stripeValue1;
				structPtr->stripe[INDEX][1] = stripeValue2;

				short tempValue[2];
				tempValue[0] = stripeValue1;
				tempValue[1] = stripeValue2;


				// decode stripe value
				printf("stripe index : %d 0 : ", INDEX);
				for (int k = 0; k < 12; k++)
				{
						short tempShort = tempValue[0] >> k;
						short andValue = tempShort & 1;

						if (andValue == 0)
						{
							structPtr->stripeMatrix[INDEX][0][k] = 0.0;
						}
						else
						{
							structPtr->stripeMatrix[INDEX][0][k] = 1.0;
						}
						printf("%d", andValue);
				}
				printf("\n");


				printf("stripe index : %d 1 : ", INDEX);
				for (int k = 0; k < 12; k++)
				{
					short tempShort = tempValue[1] >> k;
					short andValue = tempShort & 1;

					if (andValue == 0)
					{
						structPtr->stripeMatrix[INDEX][1][k] = 0.0;
					}
					else
					{
						structPtr->stripeMatrix[INDEX][1][k] = 1.0;
					}
					printf("%d", andValue);
				}
				printf("\n");



				structPtr->isFirstSetStripe[INDEX] = false;
			}
		}
		else if (strcmp(tok, "tap") == 0)
		{
			structPtr->isTapped = true;
		}



		tok = strtok_s(NULL, kugiri, &ctx);
		if (tok == NULL) { break; }
	}
}



UDPClass::UDPClass()
{
	printf("UDP class init\n");

	// set socket to null
	arg_STRUCT.receiveSocket = NULL;
	arg_STRUCT.sendSocket = NULL;

	// receive proc interval
	arg_STRUCT.mSec = 1;

	// init buffer
	char* buf = arg_STRUCT.receiveBuffer;
	memset(buf, 0, 256);

	// create mutex
	arg_STRUCT.h_mutex = CreateMutex(NULL, true, NULL);

	// init stripe value
	for (int i = 0; i < 24; i++)
	{
		arg_STRUCT.stripe[i][0] = 0;
		arg_STRUCT.stripe[i][1] = 0;
		arg_STRUCT.isFirstSetStripe[i] = true;
	}

	arg_STRUCT.isFirstSetScene[0] = true;
	arg_STRUCT.isFirstSetScene[1] = true;
	arg_STRUCT.isFirstSetScene[2] = true;

	arg_STRUCT.scene_number = 1;
	arg_STRUCT.current_objINDEX = 0;
	arg_STRUCT.current_transY = -1.0;

	arg_STRUCT.isTapped = false;

	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			arg_STRUCT.stripeMatrix[i][0][j] = 0.0;
			arg_STRUCT.stripeMatrix[i][1][j] = 0.0;
		}
	}
}


UDPClass::~UDPClass()
{
	closesocket(socket_dsc_send);
	closesocket(socket_dsc_receive);
	WSACleanup();
}




bool UDPClass::createSendSocket(const char* IP, int port)
{
	WSAData wsaData;

	// WSA startup
	int err = WSAStartup( MAKEWORD(2, 0), &wsaData);
	if (err != 0)
	{
		printf("WinSock startup fail... return\n");
		return false;
	}


	// create socket for send

	// check if socket is already opened
	if (socket_dsc_send != NULL)
	{
		closesocket(socket_dsc_send);
		socket_dsc_send = NULL;
	}

	// create socket
	socket_dsc_send = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_dsc_send == INVALID_SOCKET)
	{
		printf("create socket_send fail... return\n");
		closesocket(socket_dsc_send);
		socket_dsc_send = NULL;
		return false;
	}


	// setup address
	memset(&addr_send, 0, sizeof(addr_send));
	addr_send.sin_family = AF_INET;
	addr_send.sin_port = htons(port);
	addr_send.sin_addr.S_un.S_addr = inet_addr(IP);


	// register socket and addr to STRUCT
	arg_STRUCT.sendSocket = socket_dsc_send;
	arg_STRUCT.sendAddr = addr_send;


	return true;
}



void UDPClass::sendData(char* buffer, int length)
{


	if (socket_dsc_send != NULL)
	{
		int sendLength;
		struct sockaddr_in tempAddr = arg_STRUCT.sendAddr;

		sendLength = sendto( socket_dsc_send,
						buffer,
						length,
						0,
						(struct sockaddr*)&tempAddr,
						sizeof(tempAddr)
					);
		
	}
}



bool UDPClass::createReceiveSocket(int port)
{

	// WSA startup
	WSAData wsaData;

	int err = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (err != 0)
	{
		printf("WinSock startup fail.... return\n");
		return false;
	}



	// create socket for receive

	// check if socket is already opened
	if (socket_dsc_receive != NULL)
	{
		closesocket(socket_dsc_receive);
		socket_dsc_receive = NULL;
	}


	// create socket
	socket_dsc_receive = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_dsc_receive == INVALID_SOCKET)
	{
		printf("create socket_receive fail.... return\n");
		closesocket( socket_dsc_receive );
		socket_dsc_receive = NULL;
		return false;
	}


	// setup address
	memset(&addr_receive, 0, sizeof(addr_receive));
	addr_receive.sin_family = AF_INET;
	addr_receive.sin_port = htons(port);
	addr_receive.sin_addr.S_un.S_addr = INADDR_ANY;



	// bind socket
	if (bind(socket_dsc_receive, (struct sockaddr*)&addr_receive, sizeof(addr_receive)))
	{
		printf("bind to socket fail.... return\n");
		closesocket(socket_dsc_receive);
		socket_dsc_receive = NULL;
		return false;
	}


	// register socket and addr to STRUCT
	arg_STRUCT.receiveSocket = socket_dsc_receive;
	arg_STRUCT.receiveAddr = addr_receive;


	// create thread for receive loop

	rThread = CreateThread(
		NULL, // security
		0,	//stack size
		receiveProc,
		&arg_STRUCT, // arg
		0,	// 0 or CREATE_SUSPENDED
		&dwThreadID
	);


	return true;
}


char* UDPClass::readReceiveBuffer()
{
	if (socket_dsc_receive == NULL)
	{
		printf("receive socket is not opened.... return\n");
		return NULL;
	}

	return arg_STRUCT.receiveBuffer;

}