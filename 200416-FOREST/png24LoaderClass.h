#pragma once
#include <fstream>
#include <iostream>

// to use "htonl" function *******
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

class png24LoaderClass
{
public:

	unsigned char* PNG_dataPtr;
	unsigned char* FiltPNG_dataPtr;
	int PNG_width;
	int PNG_height;


	// for decode
	int SAMELEN_TABLE[29];
	int LEN_EX_BIT_TABLE[29];

	int DIST_TABLE[30];
	int DIST_EX_BIT_TABLE[30];


	unsigned char FIX_LIT_LEN[288];
	unsigned char FIX_LIT_HUFFMAN[288][9];
	unsigned char FIX_DIST_LEN[32];
	unsigned char FIX_DIST_HUFFMAN[32][5];

	unsigned char HC_LEN[19];
	unsigned char HC_HUFFMAN[19][7];
	unsigned char LIT_LEN[288];
	unsigned char DIST_LEN[32];
	unsigned char LIT_HUFFMAN[288][15];
	unsigned char DIST_HUFFMAN[32][15];

	png24LoaderClass();
	~png24LoaderClass();



	bool load_png24_from_path(const char* filePath);
	
private:
	bool get_compressed_data_from_IDAT(unsigned char* idatPtr, long length);
	bool inflate_compressed_data(unsigned char* compPtr, long compLength);
	unsigned char* decode_fixed_huffman(unsigned char* charPtr);
	unsigned char* decode_custom_huffman(unsigned char* charPtr);
	unsigned int reverse_bit(unsigned int num, int bitcount);

	int check_fixed_lit_huffman_identity(unsigned char* bitPtr, int bitWidth);
	int check_fixed_dist_huffman_identity(unsigned char* bitPtr, int bitWidth);
	int check_hc_huffman_identity(unsigned char* bitPtr, int bitWidth);
	int check_lit_huffman_identity(unsigned char* bitPtr, int bitWidth);
	int check_dist_huffman_identity(unsigned char* bitPtr, int bitWidth);

	int peath_algorythm(int A, int B, int C)
	{
		int p = A + B - C;
		int pa = abs(p - A);
		int pb = abs(p - B);
		int pc = abs(p - C);

		if (pa <= pb && pa <= pc)
		{
			return A;
		}


		if (pb <= pc)
		{
			return B;
		}

		return C;
	}
};

