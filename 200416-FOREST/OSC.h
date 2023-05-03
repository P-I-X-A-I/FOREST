#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mutex>
#include <thread>

#pragma comment( lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define RECV_BUF_SIZE 512
#define MAX_RECV_ARG_NUM 50
#define MAX_SEND_ARG_NUM 30
#define MAX_CALLBACK_NUM 20

class NotificationCenterClass;


enum ARG_TYPE
{
	ARG_UNDEFINED,
	ARG_INT,
	ARG_INT32, // not used
	ARG_INT64,
	ARG_FLOAT,
	ARG_DOUBLE,
	ARG_STRING,
	ARG_BOOL_TRUE,
	ARG_BOOL_FALSE,
	ARG_BLOB // not used
};




class OSC
{
public:
	OSC();
	~OSC();


	NotificationCenterClass* notification_obj;


	SOCKET SOCKET_RECEIVE_DSC;
	SOCKET SOCKET_SEND_DSC;
	//int socket_receive_dsc;
	//int socket_send_dsc;
	struct sockaddr_in addr_receive;
	struct sockaddr_in addr_send;

	// OSC address
	char* OSC_address_ptr;
	unsigned long OSC_address_LENGTH;

	// OSC send argument
	enum ARG_TYPE ARGUMENT_TYPE[MAX_SEND_ARG_NUM];
	int int_arg[MAX_SEND_ARG_NUM];
	long long long_arg[MAX_SEND_ARG_NUM];
	float float_arg[MAX_SEND_ARG_NUM];
	double double_arg[MAX_SEND_ARG_NUM];
	char* string_arg[MAX_SEND_ARG_NUM];
	int argString_len[MAX_SEND_ARG_NUM];
	int numOf_Arg;


	// OSC receive argument
	char* argument_as_string[MAX_RECV_ARG_NUM];


	// OSC send buffer
	char* sendBuffer_ptr;
	char* header_string;
	char* timestamp_data;
	//int tempINT;
	//long long tempLONG;


	// OSC receive address
	char* recv_Address[MAX_CALLBACK_NUM];
	int numOf_Callback;



	// method ***********************************************

	// create socket for send & receive
	bool set_ReceiveSocket(int portNumber);
	bool set_SendSocket(const char* IP_ADDRESS, int portNumber);
	
	// set OSC address
	void set_OSC_address(const char* addr_string);


	// add argument
	void addIntArg(int num);
	void addLongArg(long long num);
	void addFloatArg(float num);
	void addDoubleArg(double num);
	void addStringArg(const char* str);
	void clearArg();


	// get argument
	int getArgAsInt(int index);
	long long getArgAsLong(int index);
	float getArgAsFloat(int index);
	double getArgAsDouble(int index);
	char* getArgAsString(int index);


	// send data
	void sendBundleMessage();
	void sendMessage();

	// receive data
	void receiveMethod();

	void swapINT(int* ptr);
	void swapLONG(long long* ptr);
	void swapFLOAT(float* ptr);
	void swapDOUBLE(double* ptr);


	void setReceiveAddress(const char* addr);
};

