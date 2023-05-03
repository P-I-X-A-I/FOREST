#include "stdafx.h"
#include "OSC.h"
#include "NotificationCenterClass.h"


OSC::OSC()
{
	printf("OSC constructor\n");

	// init winsock
	WORD version = MAKEWORD(2, 0);
	WSADATA wsaData;
	int error;

	error = WSAStartup(version, &wsaData);

	if (error != 0)
	{
		printf("winsock initialization fail..... \n");
	}
	else
	{
		printf("winsock initialization SUCCESS!\n");
	}



	// init variables
	SOCKET_RECEIVE_DSC = INVALID_SOCKET;
	SOCKET_SEND_DSC = INVALID_SOCKET;
	//socket_receive_dsc = -1;
	//socket_send_dsc = -1;


	OSC_address_ptr = NULL;
	OSC_address_LENGTH = 0;


	for (int i = 0; i < MAX_SEND_ARG_NUM; i++)
	{
		ARGUMENT_TYPE[i] = ARG_UNDEFINED;
		int_arg[i] = 0;
		long_arg[i] = 0;
		float_arg[i] = 0.0;
		double_arg[i] = 0.0;
		string_arg[i] = NULL;
		argString_len[i] = 0;
	}
	numOf_Arg = 0;


	for (int i = 0; i < MAX_RECV_ARG_NUM; i++)
	{
		argument_as_string[i] = NULL;
	}



	// send buffer
	sendBuffer_ptr = NULL;

	header_string = (char*)malloc(8);
	memset(header_string, 0, 8);

	timestamp_data = (char*)malloc(8);
	memset(timestamp_data, 0, 8);

	char* restore_header = header_string;
	char* restore_timestamp = timestamp_data;

	*header_string = '#';	header_string++;
	*header_string = 'b';	header_string++;
	*header_string = 'u';	header_string++;
	*header_string = 'n';	header_string++;
	*header_string = 'd';	header_string++;
	*header_string = 'l';	header_string++;
	*header_string = 'e';	header_string++;

	header_string = restore_header;

	*timestamp_data = 0;	timestamp_data++;
	*timestamp_data = 0;	timestamp_data++;
	*timestamp_data = 0;	timestamp_data++;
	*timestamp_data = 0;	timestamp_data++;
	*timestamp_data = 0;	timestamp_data++;
	*timestamp_data = 0;	timestamp_data++;
	*timestamp_data = 0;	timestamp_data++;
	*timestamp_data = 1;	timestamp_data++;

	timestamp_data = restore_timestamp;


	for (int i = 0; i < MAX_CALLBACK_NUM; i++)
	{
		recv_Address[i] = NULL;
	}
	numOf_Callback = 0;

}



OSC::~OSC()
{
}


//********************** RECEIVE **********************************

bool OSC::set_ReceiveSocket(int portNumber)
{
	// check if socket is already opened or not
	if (SOCKET_RECEIVE_DSC != INVALID_SOCKET)
	{
		closesocket(SOCKET_RECEIVE_DSC);
		SOCKET_RECEIVE_DSC = INVALID_SOCKET;
		Sleep(1); //ms
	}



	// create socket
	SOCKET_RECEIVE_DSC = socket(AF_INET, SOCK_DGRAM, 0);
	if (SOCKET_RECEIVE_DSC == INVALID_SOCKET)
	{
		printf("create receive socket error.... return\n");
		SOCKET_RECEIVE_DSC = INVALID_SOCKET;
		return false;
	}
	else
	{
		printf("create receive socket SUCCESS! %d\n", (int)SOCKET_RECEIVE_DSC);
	}



	// setup address
	memset(&addr_receive, 0, sizeof(addr_receive));
	addr_receive.sin_family = AF_INET;
	addr_receive.sin_addr.S_un.S_addr = INADDR_ANY;
	addr_receive.sin_port = htons(portNumber);


	// bind socket
	int error = bind(SOCKET_RECEIVE_DSC, (struct sockaddr*)&addr_receive, sizeof(addr_receive));

	if (error)
	{
		printf("bind receive socket fail.... return\n");
		closesocket(SOCKET_RECEIVE_DSC);
		SOCKET_RECEIVE_DSC = INVALID_SOCKET;
		return false;
	}



	// create receive thread
	std::thread thread_receive( &OSC::receiveMethod, this);
	thread_receive.detach(); // detach() means separated execution

	return true;
}





void OSC::receiveMethod()
{
	char buffer[RECV_BUF_SIZE];
	char header[8] = { 0 };
	int r_num = 0;

	std::mutex MTX;

	while (1)
	{
		char* ADDRESS_STRING = NULL;
		int IDX = 0;
		int ACT_TYPE_NUM = 0;
		int ARG_TYPE[MAX_RECV_ARG_NUM];
		bool isMatch = false;
		int targetIDX = 0;
		int stringCount = 0;
		int string_padding = 0;

		// receive data
		memset(buffer, 0, RECV_BUF_SIZE);
		r_num = recvfrom(SOCKET_RECEIVE_DSC, buffer, sizeof(buffer), 0, NULL, 0);

		// debug
		//for (int i = 0; i < r_num; i++)
		//{
		//	printf("%c:", buffer[i]);
		//	if (i % 4 == 0)
		//	{
		//		printf("\n");
		//	}
		//}

		// lock
		MTX.lock();

		// check if it bundle or not
		memcpy(header, buffer, 8);


		if (strcmp(header, header_string) == 0) // bundle
		{
			// get contents size *******
			int contentSize = 0;
			memcpy(&contentSize, &buffer[16], 4);
			swapINT(&contentSize);

			//printf("content size %d\n", contentSize);
		
			// get address string *******
			int addressSize = 0;
			for (int i = 20; i < 20 + contentSize; i++)
			{
				if (buffer[i] == 0)
				{
					addressSize++;
					break;
				}
				addressSize++;
			}

			// check if address is valid or not
			if (addressSize == 1 || buffer[20] != '/')
			{
				printf("this OSC message has no valid address\n");
				goto RECEIVE_SKIP;
			}


			// get address padding
			int padding = addressSize % 4;
			if (padding != 0)
			{
				padding = 4 - padding;
			}

			addressSize += padding;



			// get argument-type size
			int typeSize = 0;
			for (int i = 0; i < contentSize - addressSize; i++)
			{
				int baseIndex = 20 + addressSize;

				// check if first element is ','
				if (i == 0 && buffer[i + baseIndex] != ',')
				{
					printf("this message has no argument type identifier..\n");
					goto RECEIVE_SKIP;
				}

				if (buffer[i + baseIndex] == 0)
				{
					typeSize++;
					break;
				}
				typeSize++;
			}// for


			//get argument type
			ACT_TYPE_NUM = typeSize - 2;// excludint "," and "end 0"
			
			if (ACT_TYPE_NUM > MAX_RECV_ARG_NUM)
			{
				printf("this OSC bundle has too many argument, error.\n");
				goto RECEIVE_SKIP;
			}

			for (int i = 0; i < ACT_TYPE_NUM; i++)
			{
				int baseIndex = 20 + addressSize + 1;
				char identifier = buffer[baseIndex+i];

				switch (identifier) {
				case 'i':
					ARG_TYPE[i] = ARG_INT;
					break;

				case 'h':
					ARG_TYPE[i] = ARG_INT64;
					break;

				case 'f':
					ARG_TYPE[i] = ARG_FLOAT;
					break;

				case 'd':
					ARG_TYPE[i] = ARG_DOUBLE;
					break;

				case 's':
					ARG_TYPE[i] = ARG_STRING;
					break;
				case 'T':
					ARG_TYPE[i] = ARG_BOOL_TRUE;
					break;
				case 'F':
					ARG_TYPE[i] = ARG_BOOL_FALSE;
					break;
				default:
					ARG_TYPE[i] = ARG_UNDEFINED;
					printf("this OSC bundle has undefined argument.\n");
					goto RECEIVE_SKIP;
					break;
				}

				//printf("identifier %c\n", identifier);
			}// for


			//
			int type_padding = typeSize % 4;
			if (type_padding != 0)
			{
				type_padding = 4 - type_padding;
			}

			typeSize += type_padding;



			// alloc address string memory
			// memory allocation will be skipped if bundle message is not valid.
			ADDRESS_STRING = (char*)malloc(addressSize);
			memcpy(ADDRESS_STRING, &buffer[20], addressSize);

			// set IDX to get real data
			IDX = 20 + addressSize + typeSize;

		}
		else // normal message ***************
		{
			int addressSize = 0;

			for (int i = 0; i < r_num; i++)
			{
				if (buffer[i] == 0)
				{
					addressSize++;
					break;
				}
				addressSize++;
			}


			// check address
			if (buffer[0] != '/')
			{
				printf("this OSC message has no valid address\n");
				goto RECEIVE_SKIP;
			}


			// address size
			int address_padding = addressSize % 4;
			if (address_padding != 0)
			{
				address_padding = 4 - address_padding;
			}
			addressSize += address_padding;


			// argument-type size *********
			int typeSize = 0;

			for (int i = 0; i < r_num - addressSize; i++)
			{
				int baseIndex = addressSize;

				// check if the first element is ','
				if (i == 0 && buffer[baseIndex] != ',')
				{
					printf("this message has no argument type identifier.\n");
					goto RECEIVE_SKIP;
				}

				if (buffer[i + baseIndex] == 0)
				{
					typeSize++;
					break;
				}
				typeSize++;
			}// for


			// get argument type
			ACT_TYPE_NUM = typeSize - 2;// excluding "," and "end 0"

			if (ACT_TYPE_NUM > MAX_RECV_ARG_NUM)
			{
				printf("this OSC message has too many argument. error\n");
				goto RECEIVE_SKIP;
			}

			for (int i = 0; i < ACT_TYPE_NUM; i++)
			{
				int baseIndex = addressSize + 1;
				char identifier = buffer[i + baseIndex];

				switch (identifier)
				{
				case 'i':
					ARG_TYPE[i] = ARG_INT;
					break;
				case 'h':
					ARG_TYPE[i] = ARG_INT64;
					break;
				case 'f':
					ARG_TYPE[i] = ARG_FLOAT;
					break;
				case 'd':
					ARG_TYPE[i] = ARG_DOUBLE;
					break;
				case 's':
					ARG_TYPE[i] = ARG_STRING;
					break;
				case 'T':
					ARG_TYPE[i] = ARG_BOOL_TRUE;
					break;
				case 'F':
					ARG_TYPE[i] = ARG_BOOL_FALSE;
					break;
				default:
					ARG_TYPE[i] = ARG_UNDEFINED;
					printf("this OSC message has undefined argument.\n");
					goto RECEIVE_SKIP;
					break;
				}
			}// for


			int type_padding = typeSize % 4;
			if (type_padding != 0)
			{
				type_padding = 4 - type_padding;
			}
			typeSize += type_padding;


			// alloc address memory
			// memory allocation will be skipped if OSC message is invalid
			ADDRESS_STRING = (char*)malloc(addressSize);
			memcpy(ADDRESS_STRING, &buffer[0], addressSize);

			// set IDX to get real data
			IDX = addressSize + typeSize;

		} // normal message



		// get real data ***********************************************
		targetIDX = IDX;
		int tempINT;
		long long tempLONG;
		float tempFLOAT;
		double tempDOUBLE;


		// clear argument as string
		for (int i = 0; i < MAX_RECV_ARG_NUM; i++)
		{
			if (argument_as_string[i] != NULL)
			{
				free(argument_as_string[i]);
				argument_as_string[i] = NULL;
			}
		}


		// check if ADDRESS is match with registered osc address
		for (int i = 0; i < numOf_Callback; i++)
		{
			if (strcmp(recv_Address[i], ADDRESS_STRING ) == 0)
			{
				isMatch = true;
				break;
			}
		}


		// get real data as string
		if (isMatch)
		{
			for (int i = 0; i < ACT_TYPE_NUM; i++)
			{
				switch (ARG_TYPE[i])
				{
				case ARG_INT:
					memcpy(&tempINT, &buffer[targetIDX], 4);
					swapINT(&tempINT);

					// int -2.147.483.648 ~ ....
					argument_as_string[i] = (char*)malloc(13);
					memset(argument_as_string[i], 0, 13);
					sprintf_s(argument_as_string[i], 13, "%d", tempINT);
					//printf("int %s\n", argument_as_string[i]);
					targetIDX += 4;
					break;

				case ARG_INT64:
					memcpy(&tempLONG, &buffer[targetIDX], 8);
					swapLONG(&tempLONG);

					argument_as_string[i] = (char*)malloc(22);
					memset(argument_as_string[i], 0, 22);
					sprintf_s(argument_as_string[i], 22, "%lld", tempLONG);
					//printf("long %s\n", argument_as_string[i]);
					targetIDX += 8;
					break;

				case ARG_FLOAT:
					memcpy(&tempFLOAT, &buffer[targetIDX], 4);
					swapFLOAT(&tempFLOAT);

					argument_as_string[i] = (char*)malloc(25);
					memset(argument_as_string[i], 0, 25);
					sprintf_s(argument_as_string[i], 25, "%f", tempFLOAT);
					//printf("float %s\n", argument_as_string[i]);
					targetIDX += 4;
					break;

				case ARG_DOUBLE:
					memcpy(&tempDOUBLE, &buffer[targetIDX], 8);
					swapDOUBLE(&tempDOUBLE);

					argument_as_string[i] = (char*)malloc(50);
					memset(argument_as_string[i], 0, 50);
					sprintf_s(argument_as_string[i], 50, "%1.10f", tempDOUBLE);
					//printf("double %s\n", argument_as_string[i]);
					targetIDX += 8;
					break;

				case ARG_STRING:
					stringCount = 0;
					string_padding = 0;

					// get length
					for (int i = 0; i < r_num - targetIDX; i++)
					{
						if (buffer[targetIDX + i] == 0)
						{
							stringCount++;
							break;
						}
						stringCount++;
					}

					// padding
					string_padding = stringCount % 4;
					if (string_padding != 0)
					{
						string_padding = 4 - string_padding;
					}

					stringCount += string_padding;

					argument_as_string[i] = (char*)malloc(stringCount);
					memset(argument_as_string[i], 0, stringCount);
					sprintf_s(argument_as_string[i], stringCount, "%s", &buffer[targetIDX]);
					//printf("string %s\n", argument_as_string[i]);
					targetIDX += stringCount;

					break;
				case ARG_BOOL_TRUE:
					printf("true");
					break;
				case ARG_BOOL_FALSE:
					printf("false,");
					break;
				case ARG_UNDEFINED:
					break;

				default:
					break;
				}
			}// for type num
		}// isMatch




		//post notification**********************************
		for (int i = 0; i < numOf_Callback; i++)
		{
			if (strcmp(recv_Address[i], ADDRESS_STRING) == 0)
			{
				//printf("ADDRESS MATCH : %d %f\n", arg1, arg2);
				notification_obj->receiveOSC(recv_Address[i]);
			}
		}





		// label for skip
		RECEIVE_SKIP :


		// dealloc memory
		if (ADDRESS_STRING != NULL)
		{
			free(ADDRESS_STRING);
		}


		// unlock
		MTX.unlock();

		Sleep(1);

	} // while
}






//********************** SEND ******************************

bool OSC::set_SendSocket(const char* IP_ADDRESS, int portNumber)
{
	// check if send socket is already opened or not
	if (SOCKET_SEND_DSC != INVALID_SOCKET)
	{
		closesocket(SOCKET_SEND_DSC);
		SOCKET_SEND_DSC = INVALID_SOCKET;
		Sleep(1);
	}

	// create socket
	SOCKET_SEND_DSC = socket(AF_INET, SOCK_DGRAM, 0);

	if (SOCKET_SEND_DSC == INVALID_SOCKET)
	{
		printf(" create send socket fail.... return\n");
		SOCKET_SEND_DSC = INVALID_SOCKET;
		return false;
	}
	else
	{
		//socket_send_dsc = (int)tempSocket;
	}


	//setup address
	memset(&addr_send, 0, sizeof(addr_send));
	addr_send.sin_family = AF_INET;
	addr_send.sin_port = htons(portNumber);
	//addr_send.sin_addr.S_un.S_addr = inet_addr(IP_ADDRESS);

	//int error = inet_pton(AF_INET, IP_ADDRESS, &addr_send.sin_addr.S_un.S_addr);
	wchar_t w_address[100];
	mbstowcs_s(NULL, w_address, IP_ADDRESS, strlen(IP_ADDRESS));
	InetPton(AF_INET, w_address, &addr_send.sin_addr.S_un.S_addr);


	return true;
}



// argument **************************************************************

void OSC::set_OSC_address(const char* addr_string)
{
	if (OSC_address_ptr != NULL)
	{
		free(OSC_address_ptr);
		OSC_address_ptr = NULL;
	}

	// get length of OSC address
	OSC_address_LENGTH = (unsigned long)strlen(addr_string) + 1; // including terminate null

	// padding
	int padding = OSC_address_LENGTH % 4;
	if (padding != 0)
	{
		padding = 4 - padding;
	}

	OSC_address_LENGTH += padding;


	// alloc memory
	OSC_address_ptr = (char*)malloc(OSC_address_LENGTH);
	memset(OSC_address_ptr, 0, OSC_address_LENGTH);

	// copy string
	strcpy_s(OSC_address_ptr, OSC_address_LENGTH, addr_string);

	printf("OSC address is set %s\n", OSC_address_ptr);
}

void OSC::addIntArg(int num)
{
	if (numOf_Arg >= MAX_SEND_ARG_NUM)
	{
		printf("too much argument\n");
		return;
	}

	// set argument
	int_arg[numOf_Arg] = num;

	// set type
	ARGUMENT_TYPE[numOf_Arg] = ARG_INT;

	numOf_Arg++;
}

void OSC::addLongArg(long long num)
{
	if (numOf_Arg >= MAX_SEND_ARG_NUM)
	{
		printf("too much argument\n");
		return;
	}

	long_arg[numOf_Arg] = num;
	ARGUMENT_TYPE[numOf_Arg] = ARG_INT64;
	numOf_Arg++;
}

void OSC::addFloatArg(float num)
{
	if (numOf_Arg >= MAX_SEND_ARG_NUM)
	{
		printf("too much argument\n");
		return;
	}

	float_arg[numOf_Arg] = num;
	ARGUMENT_TYPE[numOf_Arg] = ARG_FLOAT;
	numOf_Arg++;
}

void OSC::addDoubleArg(double num)
{
	if (numOf_Arg >= MAX_SEND_ARG_NUM)
	{
		printf("too much argument\n");
		return;
	}

	double_arg[numOf_Arg] = num;
	ARGUMENT_TYPE[numOf_Arg] = ARG_DOUBLE;
	numOf_Arg++;
}

void OSC::addStringArg(const char* str)
{
	if (numOf_Arg >= MAX_SEND_ARG_NUM)
	{
		printf("too much argument.\n");
		return;
	}

	// get length
	long long len = strlen(str) + 1;
	int padding = len % 4;
	if (padding != 0)
	{
		padding = 4 - padding;
	}

	len += padding;

	// set length of string argument
	argString_len[numOf_Arg] = (int)len;

	// alloc memory
	string_arg[numOf_Arg] = (char*)malloc(len);
	memset(string_arg[numOf_Arg], 0, len);

	// copy string
	strcpy_s(string_arg[numOf_Arg], len, str);

	// set type
	ARGUMENT_TYPE[numOf_Arg] = ARG_STRING;

	numOf_Arg++;
}


void OSC::clearArg()
{
	numOf_Arg = 0;

	for (int i = 0; i < MAX_SEND_ARG_NUM; i++)
	{
		ARGUMENT_TYPE[i] = ARG_UNDEFINED;
		int_arg[i] = 0;
		long_arg[i] = 0;
		float_arg[i] = 0.0;
		double_arg[i] = 0.0;

		argString_len[i] = 0;
		if (string_arg[i] != NULL)
		{
			free(string_arg[i]);
			string_arg[i] = NULL;
		}
	}
}



// get argument ***********************************

int OSC::getArgAsInt(int index)
{
	if (index >= MAX_RECV_ARG_NUM)
	{
		printf("this index is over the maximum limit\n");
		return 0;
	}

	if (argument_as_string[index] == NULL)
	{
		printf("[int] no argument at the index : %d\n", index);
		return 0;
	}

	int value = atoi(argument_as_string[index]);
	return value;
}

long long OSC::getArgAsLong(int index)
{
	if (index >= MAX_RECV_ARG_NUM)
	{
		printf("this index is over maximam limit\n");
		return 0;
	}

	if (argument_as_string[index] == NULL)
	{
		printf("[long] no argument at the index :%d\n", index);
		return 0;
	}

	long long value = atol(argument_as_string[index]);
	return value;
}



float OSC::getArgAsFloat(int index)
{
	if (index >= MAX_RECV_ARG_NUM)
	{
		printf("this index is over the maximam limit\n");
		return 0.0;
	}

	if (argument_as_string[index] == NULL)
	{
		printf("[float] no argument at the index : %d\n", index);
		return 0.0;
	}

	float value = (float)atof(argument_as_string[index]);
	return value;
}


double OSC::getArgAsDouble(int index)
{
	if (index >= MAX_RECV_ARG_NUM)
	{
		printf("this index is over the maximam limit\n");
		return 0.0;
	}

	if (argument_as_string[index] == NULL)
	{
		printf("[double] no argument at the index : %d\n", index);
		return 0.0;
	}


	double value = atof(argument_as_string[index]);
	return value;
}


char* OSC::getArgAsString(int index)
{
	if (index >= MAX_RECV_ARG_NUM)
	{
		printf("this index is over the maximam limit\n");
		return NULL;
	}

	if (argument_as_string[index] == NULL)
	{
		printf("[string] no argument at the index : %d\n", index);
		return NULL;
	}

	return argument_as_string[index];
}




// ****************** send data ****************************
void OSC::sendBundleMessage()
{
	int senBuf_size = 0;

	// free buffer
	if (sendBuffer_ptr != NULL)
	{
		free(sendBuffer_ptr);
		sendBuffer_ptr = NULL;
	}

	// count buffer size
	senBuf_size = 20;

	// address string
	if (OSC_address_LENGTH == 0)
	{
		printf("OSC address is not set, return\n");
		return;
	}

	senBuf_size += OSC_address_LENGTH;


	// num of argument
	int argSize = numOf_Arg + 2; // including , and end 0
	int arg_padding = argSize % 4;

	if (arg_padding != 0)
	{
		arg_padding = 4 - arg_padding;
	}

	argSize += arg_padding;
	senBuf_size += argSize;


	// argument type
	for (int i = 0; i < numOf_Arg; i++)
	{
		switch (ARGUMENT_TYPE[i]) 
		{
		case ARG_INT:
			senBuf_size += sizeof(int);
			break;

		case ARG_INT64:
			senBuf_size += sizeof(long long);
			break;

		case ARG_FLOAT:
			senBuf_size += sizeof(float);
			break;

		case ARG_DOUBLE:
			senBuf_size += sizeof(double);
			break;

		case ARG_STRING:
			senBuf_size += argString_len[i];
			break;

		default:
			break;
		}// switch
	}//for


	// alloc memory of sendbuffer
	sendBuffer_ptr = (char*)malloc(senBuf_size);
	memset(sendBuffer_ptr, 0, senBuf_size);

	char* restore_ptr = sendBuffer_ptr;


	// copy header
	memcpy(sendBuffer_ptr, header_string, 8);
	sendBuffer_ptr += 8;


	// copy timestamp
	memcpy(sendBuffer_ptr, timestamp_data, 8);
	sendBuffer_ptr += 8;


	// copy contents size
	int contents_size = senBuf_size - 20;
	swapINT(&contents_size);
	memcpy(sendBuffer_ptr, &contents_size, 4);
	sendBuffer_ptr += 4;

	// copy address string
	memcpy(sendBuffer_ptr, OSC_address_ptr, OSC_address_LENGTH);
	sendBuffer_ptr += OSC_address_LENGTH;


	// copy argument type
	*sendBuffer_ptr = ',';	sendBuffer_ptr++;

	for (int i = 0; i < numOf_Arg; i++)
	{
		switch (ARGUMENT_TYPE[i])
		{
		case ARG_INT:
			*sendBuffer_ptr = 'i';
			break;

		case ARG_INT64:
			*sendBuffer_ptr = 'h';
			break;
			
		case ARG_FLOAT:
			*sendBuffer_ptr = 'f';
			break;

		case ARG_DOUBLE:
			*sendBuffer_ptr = 'd';
			break;

		case ARG_STRING:
			*sendBuffer_ptr = 's';
			break;

		default:
			break;
		}

		sendBuffer_ptr++;
	}// for

	*sendBuffer_ptr = 0;
	sendBuffer_ptr++;

	sendBuffer_ptr += arg_padding;


	// real data
	int tempINT;
	long long tempLONG;
	float tempFLOAT;
	double tempDOUBLE;

	for (int i = 0; i < numOf_Arg; i++)
	{
		switch (ARGUMENT_TYPE[i])
		{
		case ARG_INT:
			tempINT = int_arg[i];
			swapINT(&tempINT);
			//printf("temp INT %d\n", tempINT);
			memcpy(sendBuffer_ptr, &tempINT, 4);
			sendBuffer_ptr += 4;
			break;
		case ARG_INT64:
			tempLONG = long_arg[i];
			swapLONG(&tempLONG);
			//printf("temp LONG %lld\n", tempLONG);
			memcpy(sendBuffer_ptr, &tempLONG, 8);
			sendBuffer_ptr += 8;
			break;
		case ARG_FLOAT:
			tempFLOAT = float_arg[i];
			swapFLOAT(&tempFLOAT);
			//printf("float size %d\n", sizeof(float));
			memcpy(sendBuffer_ptr, &tempFLOAT, 4);
			sendBuffer_ptr += 4;
			break;
		case ARG_DOUBLE:
			tempDOUBLE = double_arg[i];
			swapDOUBLE(&tempDOUBLE);
			//printf("double size %d\n", sizeof(double));
			memcpy(sendBuffer_ptr, &tempDOUBLE, 8);
			sendBuffer_ptr += 8;
			break;
		case ARG_STRING:
			memcpy(sendBuffer_ptr, string_arg[i], argString_len[i]);
			sendBuffer_ptr += argString_len[i];
			break;
		}// switch
	}// for


	// restore pointer
	sendBuffer_ptr = restore_ptr;

	// send buffer
	sendto(SOCKET_SEND_DSC,
		sendBuffer_ptr,
		senBuf_size,
		0,
		(struct sockaddr*)&addr_send,
		sizeof(addr_send));
}





void OSC::sendMessage()
{
	int sendBuf_size = 0;


	// free buffer
	if (sendBuffer_ptr != NULL)
	{
		free(sendBuffer_ptr);
		sendBuffer_ptr = NULL;
	}


	// check address string
	if (OSC_address_LENGTH == 0)
	{
		printf("ERROR, no osc address is set. return.\n");
		return;
	}


	sendBuf_size += OSC_address_LENGTH;



	// num of argument
	int argSize = numOf_Arg + 2; // including , and terminate 0
	int arg_padding = argSize % 4;

	if (arg_padding != 0)
	{
		arg_padding = 4 - arg_padding;
	}

	argSize += arg_padding;
	sendBuf_size += argSize;



	// argument type
	for (int i = 0; i < numOf_Arg; i++)
	{
		switch (ARGUMENT_TYPE[i]) {
		
		case ARG_INT:
			sendBuf_size += sizeof(int);
			break;

		case ARG_INT64:
			sendBuf_size += sizeof(long long);
			break;
		case ARG_FLOAT:
			sendBuf_size += sizeof(float);
			break;
		case ARG_DOUBLE:
			sendBuf_size += sizeof(double);
			break;
		case ARG_STRING:
			sendBuf_size += argString_len[i];
			break;
		default:
			break;
		}// switch
	} // for


	// alloc memory of send buffer
	sendBuffer_ptr = (char*)malloc(sendBuf_size);
	memset(sendBuffer_ptr, 0, sendBuf_size);

	char* restore_ptr = sendBuffer_ptr;


	// copy address string
	memcpy(sendBuffer_ptr, OSC_address_ptr, OSC_address_LENGTH);
	sendBuffer_ptr += OSC_address_LENGTH;

	// copy argument type
	*sendBuffer_ptr = ','; sendBuffer_ptr++;

	for (int i = 0; i < numOf_Arg; i++)
	{
		switch (ARGUMENT_TYPE[i])
		{
		case ARG_INT:
			*sendBuffer_ptr = 'i';
			break;
		case ARG_INT64:
			*sendBuffer_ptr = 'h';
			break;
		case ARG_FLOAT:
			*sendBuffer_ptr = 'f';
			break;
		case ARG_DOUBLE:
			*sendBuffer_ptr = 'd';
			break;
		case ARG_STRING:
			*sendBuffer_ptr = 's';
			break;
		default:
			break;
		}// switch

		sendBuffer_ptr++;
	}// for


	*sendBuffer_ptr = 0; // end 0
	sendBuffer_ptr++;

	sendBuffer_ptr += arg_padding;



	// real data
	int tempINT;
	long long tempLONG;
	float tempFLOAT;
	double tempDOUBLE;

	for (int i = 0; i < numOf_Arg; i++)
	{
		switch (ARGUMENT_TYPE[i])
		{
			case ARG_INT:
				tempINT = int_arg[i];
				swapINT(&tempINT);
				//printf("temp INT %d\n", tempINT);
				memcpy(sendBuffer_ptr, &tempINT, 4);
				sendBuffer_ptr += 4;
				break;
			case ARG_INT64:
				tempLONG = long_arg[i];
				swapLONG(&tempLONG);
				//printf("temp LONG %lld\n", tempLONG);
				memcpy(sendBuffer_ptr, &tempLONG, 8);
				sendBuffer_ptr += 8;
				break;
			case ARG_FLOAT:
				tempFLOAT = float_arg[i];
				swapFLOAT(&tempFLOAT);
				//printf("float size %d\n", sizeof(float));
				memcpy(sendBuffer_ptr, &tempFLOAT, 4);
				sendBuffer_ptr += 4;
				break;
			case ARG_DOUBLE:
				tempDOUBLE = double_arg[i];
				swapDOUBLE(&tempDOUBLE);
				//printf("double size %d\n", sizeof(double));
				memcpy(sendBuffer_ptr, &tempDOUBLE, 8);
				sendBuffer_ptr += 8;
				break;
			case ARG_STRING:
				memcpy(sendBuffer_ptr, string_arg[i], argString_len[i]);
				sendBuffer_ptr += argString_len[i];
				break;
		}// switch
	}// for



	// restore pointer
	sendBuffer_ptr = restore_ptr;


	// send buffer
	int sendSize = sendto(SOCKET_SEND_DSC,
		sendBuffer_ptr,
		sendBuf_size,
		0,
		(struct sockaddr*)&addr_send,
		sizeof(addr_send));

	printf("sendsize %d\n", sendSize);
}


void OSC::swapINT(int* ptr)
{
	char inVal[4] = {0};
	char swapVal[4] = {0};

	memcpy(inVal, ptr, 4);

	swapVal[0] = inVal[3];
	swapVal[1] = inVal[2];
	swapVal[2] = inVal[1];
	swapVal[3] = inVal[0];

	memcpy(ptr, swapVal, 4);

}

void OSC::swapLONG(long long* ptr)
{
	char inVal[8] = {0};
	char swapVal[8] = {0};

	memcpy(inVal, ptr, 8);

	swapVal[0] = inVal[7];
	swapVal[1] = inVal[6];
	swapVal[2] = inVal[5];
	swapVal[3] = inVal[4];
	swapVal[4] = inVal[3];
	swapVal[5] = inVal[2];
	swapVal[6] = inVal[1];
	swapVal[7] = inVal[0];

	memcpy(ptr, swapVal, 8);
}

void OSC::swapFLOAT(float* ptr)
{
	char inVal[4];
	char swapVal[4];

	memcpy(inVal, ptr, 4);

	swapVal[0] = inVal[3];
	swapVal[1] = inVal[2];
	swapVal[2] = inVal[1];
	swapVal[3] = inVal[0];

	memcpy(ptr, swapVal, 4);
}

void OSC::swapDOUBLE(double* ptr)
{
	char inVal[8] = { 0 };
	char swapVal[8] = { 0 };

	memcpy(inVal, ptr, 8);

	swapVal[0] = inVal[7];
	swapVal[1] = inVal[6];
	swapVal[2] = inVal[5];
	swapVal[3] = inVal[4];
	swapVal[4] = inVal[3];
	swapVal[5] = inVal[2];
	swapVal[6] = inVal[1];
	swapVal[7] = inVal[0];

	memcpy(ptr, swapVal, 8);
}


void OSC::setReceiveAddress(const char* addr)
{
	if (numOf_Callback < MAX_CALLBACK_NUM)
	{
		int len = strlen(addr);
		recv_Address[numOf_Callback] = (char*)malloc(len+1);
		memset(recv_Address[numOf_Callback], 0, len + 1);
		memcpy(recv_Address[numOf_Callback], addr, len);
		numOf_Callback++;
	}
	else
	{
		printf("over limitation of registable address.\n");
	}
}