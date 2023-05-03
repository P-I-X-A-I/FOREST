#include "stdafx.h"
#include "png24LoaderClass.h"


png24LoaderClass::png24LoaderClass()
{
	std::cout << "png24LoaderClass init" << std::endl;

	FiltPNG_dataPtr = NULL;
	PNG_width = 0;
	PNG_height = 0;

	SAMELEN_TABLE[0] = 3;	LEN_EX_BIT_TABLE[0] = 0;
	SAMELEN_TABLE[1] = 4;	LEN_EX_BIT_TABLE[1] = 0;
	SAMELEN_TABLE[2] = 5;	LEN_EX_BIT_TABLE[2] = 0;
	SAMELEN_TABLE[3] = 6;	LEN_EX_BIT_TABLE[3] = 0;
	SAMELEN_TABLE[4] = 7;	LEN_EX_BIT_TABLE[4] = 0;
	SAMELEN_TABLE[5] = 8;	LEN_EX_BIT_TABLE[5] = 0;
	SAMELEN_TABLE[6] = 9;	LEN_EX_BIT_TABLE[6] = 0;
	SAMELEN_TABLE[7] = 10;	LEN_EX_BIT_TABLE[7] = 0;
	SAMELEN_TABLE[8] = 11;	LEN_EX_BIT_TABLE[8] = 1;
	SAMELEN_TABLE[9] = 13;	LEN_EX_BIT_TABLE[9] = 1;

	SAMELEN_TABLE[10] = 15;	LEN_EX_BIT_TABLE[10] = 1;
	SAMELEN_TABLE[11] = 17;	LEN_EX_BIT_TABLE[11] = 1;
	SAMELEN_TABLE[12] = 19;	LEN_EX_BIT_TABLE[12] = 2;
	SAMELEN_TABLE[13] = 23;	LEN_EX_BIT_TABLE[13] = 2;
	SAMELEN_TABLE[14] = 27;	LEN_EX_BIT_TABLE[14] = 2;
	SAMELEN_TABLE[15] = 31;	LEN_EX_BIT_TABLE[15] = 2;
	SAMELEN_TABLE[16] = 35;	LEN_EX_BIT_TABLE[16] = 3;
	SAMELEN_TABLE[17] = 43;	LEN_EX_BIT_TABLE[17] = 3;
	SAMELEN_TABLE[18] = 51;	LEN_EX_BIT_TABLE[18] = 3;
	SAMELEN_TABLE[19] = 59;	LEN_EX_BIT_TABLE[19] = 3;

	SAMELEN_TABLE[20] = 67;	LEN_EX_BIT_TABLE[20] = 4;
	SAMELEN_TABLE[21] = 83;	LEN_EX_BIT_TABLE[21] = 4;
	SAMELEN_TABLE[22] = 99;	LEN_EX_BIT_TABLE[22] = 4;
	SAMELEN_TABLE[23] = 115;	LEN_EX_BIT_TABLE[23] = 4;
	SAMELEN_TABLE[24] = 131;	LEN_EX_BIT_TABLE[24] = 5;
	SAMELEN_TABLE[25] = 163;	LEN_EX_BIT_TABLE[25] = 5;
	SAMELEN_TABLE[26] = 195;	LEN_EX_BIT_TABLE[26] = 5;
	SAMELEN_TABLE[27] = 227;	LEN_EX_BIT_TABLE[27] = 5;
	SAMELEN_TABLE[28] = 258;	LEN_EX_BIT_TABLE[28] = 0;


	DIST_TABLE[0] = 1;		DIST_EX_BIT_TABLE[0] = 0;
	DIST_TABLE[1] = 2;		DIST_EX_BIT_TABLE[1] = 0;
	DIST_TABLE[2] = 3;		DIST_EX_BIT_TABLE[2] = 0;
	DIST_TABLE[3] = 4;		DIST_EX_BIT_TABLE[3] = 0;
	DIST_TABLE[4] = 5;		DIST_EX_BIT_TABLE[4] = 1;
	DIST_TABLE[5] = 7;		DIST_EX_BIT_TABLE[5] = 1;
	DIST_TABLE[6] = 9;		DIST_EX_BIT_TABLE[6] = 2;
	DIST_TABLE[7] = 13;		DIST_EX_BIT_TABLE[7] = 2;
	DIST_TABLE[8] = 17;		DIST_EX_BIT_TABLE[8] = 3;
	DIST_TABLE[9] = 25;		DIST_EX_BIT_TABLE[9] = 3;

	DIST_TABLE[10] = 33;		DIST_EX_BIT_TABLE[10] = 4;
	DIST_TABLE[11] = 49;		DIST_EX_BIT_TABLE[11] = 4;
	DIST_TABLE[12] = 65;		DIST_EX_BIT_TABLE[12] = 5;
	DIST_TABLE[13] = 97;		DIST_EX_BIT_TABLE[13] = 5;
	DIST_TABLE[14] = 129;		DIST_EX_BIT_TABLE[14] = 6;
	DIST_TABLE[15] = 193;		DIST_EX_BIT_TABLE[15] = 6;
	DIST_TABLE[16] = 257;		DIST_EX_BIT_TABLE[16] = 7;
	DIST_TABLE[17] = 385;		DIST_EX_BIT_TABLE[17] = 7;
	DIST_TABLE[18] = 513;		DIST_EX_BIT_TABLE[18] = 8;
	DIST_TABLE[19] = 769;		DIST_EX_BIT_TABLE[19] = 8;
	
	DIST_TABLE[20] = 1025;		DIST_EX_BIT_TABLE[20] = 9;
	DIST_TABLE[21] = 1537;		DIST_EX_BIT_TABLE[21] = 9;
	DIST_TABLE[22] = 2049;		DIST_EX_BIT_TABLE[22] = 10;
	DIST_TABLE[23] = 3073;		DIST_EX_BIT_TABLE[23] = 10;
	DIST_TABLE[24] = 4097;		DIST_EX_BIT_TABLE[24] = 11;
	DIST_TABLE[25] = 6145;		DIST_EX_BIT_TABLE[25] = 11;
	DIST_TABLE[26] = 8193;		DIST_EX_BIT_TABLE[26] = 12;
	DIST_TABLE[27] = 12289;		DIST_EX_BIT_TABLE[27] = 12;
	DIST_TABLE[28] = 16385;		DIST_EX_BIT_TABLE[28] = 13;
	DIST_TABLE[29] = 24577;		DIST_EX_BIT_TABLE[29] = 13;


	// create fixed huffman table

	// init
	for (int i = 0; i < 286; i++)
	{
		for (int k = 0; k < 9; k++)
		{
			FIX_LIT_HUFFMAN[i][k] = 255;
		}
	}

	// 0 - 143 : 8bit, 00110000 - 10111111
	unsigned int base = 48; // 00110000
	for (int i = 0 ; i <= 143; i++)
	{
		FIX_LIT_LEN[i] = 8; // 8bit bitwidth

		unsigned int cp = base;

		for (int k = 7; k >= 0; k--)
		{
			FIX_LIT_HUFFMAN[i][k] = cp & 1;
			cp >>= 1;
		}

		base++;
	}

	// 144 - 255 9bit
	base = 256 + 128 + 16; // 110010000
	for (int i = 144; i <= 255; i++)
	{
		FIX_LIT_LEN[i] = 9;

		unsigned int cp = base;

		for (int k = 8; k >= 0; k--)
		{
			FIX_LIT_HUFFMAN[i][k] = cp & 1;
			cp >>= 1;
		}

		base++;
	}

	// 256 - 279 7bit
	base = 0; //0000000
	for (int i = 256; i <= 279; i++)
	{
		FIX_LIT_LEN[i] = 7;

		unsigned int cp = base;

		for (int k = 6; k >= 0; k--)
		{
			FIX_LIT_HUFFMAN[i][k] = cp & 1;
			cp >>= 1;
		}

		base++;
	}


	// 280 - 287 8bit
	base = 128 + 64; // 11000000
	for (int i = 280; i <= 287; i++)
	{
		FIX_LIT_LEN[i] = 8;

		unsigned int cp = base;

		for (int k = 7; k >= 0; k--)
		{
			FIX_LIT_HUFFMAN[i][k] = cp & 1;
			cp >>= 1;
		}
		base++;
	}


	// fixd dist huffman
	base = 0;
	for (int i = 0; i <= 31; i++)
	{
		FIX_DIST_LEN[i] = 5;

		unsigned int cp = base;

		for (int k = 4; k >= 0; k--)
		{
			FIX_DIST_HUFFMAN[i][k] = cp & 1;
			cp >>= 1;
		}

		base++;
	}
}


png24LoaderClass::~png24LoaderClass()
{
}




bool png24LoaderClass::load_png24_from_path(const char* filePath)
{
	if (FiltPNG_dataPtr != NULL)
	{
		free(FiltPNG_dataPtr);
		PNG_width = 0;
		PNG_height = 0;
	}



	// clear HUFFMAN table
	
	for (int i = 0; i < 19; i++)
	{
		HC_LEN[i] = 255;
		for (int j = 0; j < 7; j++)
		{
			HC_HUFFMAN[i][j] = 255;
		}
	}
	
	for (int i = 0; i < 286; i++)
	{
		LIT_LEN[i] = 255;
		for (int j = 0; j < 15; j++)
		{
			LIT_HUFFMAN[i][j] = 255;
		}
	}

	for (int i = 0; i < 32 ; i++)
	{
		DIST_LEN[i] = 255;
		for (int j = 0; j < 15; j++)
		{
			DIST_HUFFMAN[i][j] = 255;
		}
	}



	// load file
	std::ifstream sFile;
	FILE* fh;
	errno_t error;


	// open file
	error = fopen_s(&fh, filePath, "rb"); // read as binary

	if (error != 0)
	{
		std::cout << "PNG file path is invalid, can't open this file.... return" << std::endl;
		return false;
	}



	// set file's position to end of the file
	fseek(fh, 0, SEEK_END);



	// get file length
	fpos_t fLength = ftell(fh);
	if (fLength == 0)
	{
		std::cout << "this PNG file data length is 0.... return" << std::endl;
		fclose(fh);
		return false;
	}
	else
	{
		std::cout << "\n" << filePath << " : file length : " << fLength << std::endl;
	}



	// reset file's position to the beginning
	fseek(fh, 0, SEEK_SET);



	//*********************** PNG **********************************
	// check png header signature
	//std::cout << "\n*** CHECK PNG SIGNATURE ***" << std::endl;
	unsigned char header[8];
	unsigned char signature[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };

	fread_s(header, 8, 8, 1, fh);

	bool isPng = true;
	for (int i = 0; i < 8; i++)
	{
		if (header[i] != signature[i])
		{
			isPng = false;
			std::cout << "this file don't have PNG Signature... return" << std::endl;
			fclose(fh);
			return false;
		}
	}



	// check IHDR chunk ( the first chunk )
	//std::cout << "\n*** CHECK IHDR CHUNK ***" << std::endl;


	// chunk length ( 4byte )
	uint32_t IHDR_length;
	fread_s(&IHDR_length, 4, 4, 1, fh);
	// byte swap from network order ( big endian ) to host order
	IHDR_length = ntohl(IHDR_length);
	//std::cout << " -chunk length : " << IHDR_length << std::endl;


	// chunk type ( 4byte )
	unsigned char IHDR_type[5] = { 0 };
	fread_s(IHDR_type, 4, 4, 1, fh);
	//std::cout << " -chunk type : " << IHDR_type << std::endl;

	// width and height ( 4byte, 4byte )
	uint32_t IHDR_width, IHDR_height;
	fread_s(&IHDR_width, 4, 4, 1, fh);
	fread_s(&IHDR_height, 4, 4, 1, fh);

	PNG_width = ntohl(IHDR_width);
	PNG_height = ntohl(IHDR_height);
	std::cout << " -width and height : " << PNG_width << " : " << PNG_height << std::endl;


	// bit depth ( 1byte )
	uint8_t IHDR_bit_depth;
	fread_s(&IHDR_bit_depth, 1, 1, 1, fh);

	// color type
	uint8_t IHDR_color_type;
	fread_s(&IHDR_color_type, 1, 1, 1, fh);

	// check PNG type
	switch (IHDR_color_type)
	{
	case 0:
		std::cout << " -color_type : Grayscale" << std::endl;
		break;
	case 2:
		std::cout << " -color_type : RGB" << std::endl;
		break;
	case 3:
		std::cout << " -color_type : Index color" << std::endl;
		break;
	case 4:
		std::cout << " -color_type : Grayscale with Alpha" << std::endl;
		break;
	case 6:
		std::cout << " -color_type : RGBA" << std::endl;
		break;
	}

	// only png 24 is supported
	if (IHDR_color_type != 6 || IHDR_bit_depth != 8)
	{
		std::cout << "only RGBA 8bit PNG-24 is supported....return" << std::endl;
		fclose(fh);
		return false;
	}



	//********************* alloc pointer for final PNG data **************
	long pngsize = (PNG_width+1) * PNG_height * 4 * sizeof(unsigned char);

	FiltPNG_dataPtr = (unsigned char*)malloc( pngsize );
	unsigned char* restore_FiltPNG = FiltPNG_dataPtr;
	//**********************************************************************


	// compression method ( always 0 )
	uint8_t IHDR_compression;
	fread_s(&IHDR_compression, 1, 1, 1, fh);


	// filter method ( always 0 )
	uint8_t IHDR_filter;
	fread_s(&IHDR_filter, 1, 1, 1, fh);

	// interlaced
	uint8_t IHDR_interlace;
	fread_s(&IHDR_interlace, 1, 1, 1, fh);


	if (IHDR_interlace != 0)
	{
		std::cout << "Interlaced PNG is unsupported...return" << std::endl;
		fclose(fh);
		return false;
	}


	// skip CRC checksum
	uint32_t IHDR_CRC;
	fread_s(&IHDR_CRC, 4, 4, 1, fh);




	// *************** FIND IDAT CHUNK ***************
	//std::cout << "\n*** FIND IDAT CHUNK ***" << std::endl;

	uint8_t* IDAT_ptr = NULL;
	long IDAT_length = 0;

	bool loop = true;
	int IDAT_COUNT = 0;

	while (loop)
	{
		uint32_t LEN;

		// chunk length
		fread_s(&LEN, 4, 4, 1, fh);
		LEN = ntohl(LEN); //length of data ( byte )

		// chunk type
		char type[5] = { 0 };
		fread_s(type, 4, 4, 1, fh);
		//std::cout << " -chunk type : " << type << std::endl;

		// chunk contents
		unsigned char* tempData = NULL;

		if (LEN != 0)
		{
			tempData = (unsigned char*)malloc(LEN);
			fread_s(tempData, LEN, LEN, 1, fh);
		}


		// CRC checksum ( skip )
		uint32_t CRC;
		fread_s(&CRC, 4, 4, 1, fh);



		if (strcmp(type, "IDAT") == 0)
		{
			// copy data into IDAT_ptr
			IDAT_ptr = (uint8_t*)malloc(LEN);
			memcpy(IDAT_ptr, tempData, LEN);
			IDAT_length = LEN; // bytes

			free(tempData);

			IDAT_COUNT++;

			if (IDAT_COUNT > 1)
			{
				std::cout << " -find second IDAT chunk...." << std::endl;
				std::cout << " -Separate IDAT chunk is not supported...." << std::endl;
				free(IDAT_ptr);
				fclose(fh);
				return false;
			}
		}
		else if (strcmp(type, "IEND") == 0)
		{
			loop = false;
			free(tempData);
			break;
		}
		else
		{
			free(tempData);
		}

	} // while


	//*** close file *****
	fclose(fh);



	if (IDAT_COUNT == 0)
	{
		std::cout << "NO IDAT chunk found.... return" << std::endl;
		return false;
	}



	// get IDAT data into IDAT_ptr
	//std::cout << "IDAT length : " << IDAT_length << std::endl;



	//***************************************************
	bool yn = this->get_compressed_data_from_IDAT(IDAT_ptr, IDAT_length);
	//***************************************************


	if (yn)
	{
		//std::cout << "get compressed data from IDAT SUCCESS" << std::endl;
	}


	// decode filterling **************************************************
	// restore pointer
	FiltPNG_dataPtr = restore_FiltPNG;

	// alloc final PNG memory
	PNG_dataPtr = (unsigned char*)malloc(PNG_width*PNG_height * 4);
	unsigned char* restorePNG = PNG_dataPtr;


	// copy data
	int prevCol[4];
	int upCol[4];
	int upLeftCol[4];
	int peath[4];
	int tempCol[4];
	unsigned char* upPNG_ptr;

	for (int h = 0; h < PNG_height; h++)
	{
		unsigned char FILTER = *FiltPNG_dataPtr;
		FiltPNG_dataPtr++;

		switch (FILTER)
		{
		case 0: // none
			for (int w = 0; w < PNG_width; w++)
			{
				for (int CH = 0; CH < 4; CH++)
				{
					*PNG_dataPtr = *FiltPNG_dataPtr;
					PNG_dataPtr++;
					FiltPNG_dataPtr++;
				}
			}
			break;

		case 1: // sub
			// first pixel
			prevCol[0] = 0;
			prevCol[1] = 0;
			prevCol[2] = 0;
			prevCol[3] = 0;

			for (int w = 0; w < PNG_width; w++)
			{
				tempCol[0] = *FiltPNG_dataPtr; FiltPNG_dataPtr++;
				tempCol[1] = *FiltPNG_dataPtr; FiltPNG_dataPtr++;
				tempCol[2] = *FiltPNG_dataPtr; FiltPNG_dataPtr++;
				tempCol[3] = *FiltPNG_dataPtr; FiltPNG_dataPtr++;

				*PNG_dataPtr = (unsigned char)((tempCol[0] + prevCol[0]) % 256);
				prevCol[0] = *PNG_dataPtr;
				PNG_dataPtr++;
				*PNG_dataPtr = (unsigned char)((tempCol[1] + prevCol[1]) % 256);
				prevCol[1] = *PNG_dataPtr;
				PNG_dataPtr++;
				*PNG_dataPtr = (unsigned char)((tempCol[2] + prevCol[2]) % 256);
				prevCol[2] = *PNG_dataPtr;
				PNG_dataPtr++;
				*PNG_dataPtr = (unsigned char)((tempCol[3] + prevCol[3]) % 256);
				prevCol[3] = *PNG_dataPtr;
				PNG_dataPtr++;
			}
			break;

		case 2: // up
			upPNG_ptr = PNG_dataPtr - (PNG_width * 4);

			for (int w = 0; w < PNG_width; w++)
			{
				upCol[0] = *upPNG_ptr;	upPNG_ptr++;
				upCol[1] = *upPNG_ptr;	upPNG_ptr++;
				upCol[2] = *upPNG_ptr;	upPNG_ptr++;
				upCol[3] = *upPNG_ptr;	upPNG_ptr++;

				tempCol[0] = *FiltPNG_dataPtr;	FiltPNG_dataPtr++;
				tempCol[1] = *FiltPNG_dataPtr;	FiltPNG_dataPtr++;
				tempCol[2] = *FiltPNG_dataPtr;	FiltPNG_dataPtr++;
				tempCol[3] = *FiltPNG_dataPtr;	FiltPNG_dataPtr++;

				*PNG_dataPtr = (unsigned char)((upCol[0] + tempCol[0]) % 256);
				PNG_dataPtr++;
				*PNG_dataPtr = (unsigned char)((upCol[1] + tempCol[1]) % 256);
				PNG_dataPtr++;
				*PNG_dataPtr = (unsigned char)((upCol[2] + tempCol[2]) % 256);
				PNG_dataPtr++;
				*PNG_dataPtr = (unsigned char)((upCol[3] + tempCol[3]) % 256);
				PNG_dataPtr++;

			}

			break; 

		case 3: // average

			prevCol[0] = 0;
			prevCol[1] = 0;
			prevCol[2] = 0;
			prevCol[3] = 0;

			upPNG_ptr = PNG_dataPtr - (PNG_width * 4);

			for (int w = 0; w < PNG_width; w++)
			{
				upCol[0] = *upPNG_ptr;	upPNG_ptr++;
				upCol[1] = *upPNG_ptr;	upPNG_ptr++;
				upCol[2] = *upPNG_ptr;	upPNG_ptr++;
				upCol[3] = *upPNG_ptr;	upPNG_ptr++;

				tempCol[0] = *FiltPNG_dataPtr;	FiltPNG_dataPtr++;
				tempCol[1] = *FiltPNG_dataPtr;	FiltPNG_dataPtr++;
				tempCol[2] = *FiltPNG_dataPtr;	FiltPNG_dataPtr++;
				tempCol[3] = *FiltPNG_dataPtr;	FiltPNG_dataPtr++;

				*PNG_dataPtr = (unsigned char)(( tempCol[0] + (upCol[0] + prevCol[0]) / 2 ) % 256);
				prevCol[0] = *PNG_dataPtr;
				PNG_dataPtr++;
				*PNG_dataPtr = (unsigned char)(( tempCol[1] + (upCol[1] + prevCol[1]) / 2) % 256);
				prevCol[1] = *PNG_dataPtr;
				PNG_dataPtr++;
				*PNG_dataPtr = (unsigned char)((tempCol[2] + (upCol[2] + prevCol[2]) / 2) % 256);
				prevCol[2] = *PNG_dataPtr;
				PNG_dataPtr++;
				*PNG_dataPtr = (unsigned char)((tempCol[3] + (upCol[3] + prevCol[3]) / 2) % 256);
				prevCol[3] = *PNG_dataPtr;
				PNG_dataPtr++;
			}

			break;


		case 4: // peath algorythm
			upPNG_ptr = PNG_dataPtr - (PNG_width * 4);

			prevCol[0] = 0;
			prevCol[1] = 0;
			prevCol[2] = 0;
			prevCol[3] = 0;
			upLeftCol[0] = 0;
			upLeftCol[1] = 0;
			upLeftCol[2] = 0;
			upLeftCol[3] = 0;

			for (int w = 0; w < PNG_width; w++)
			{
				upCol[0] = *upPNG_ptr;	upPNG_ptr++;
				upCol[1] = *upPNG_ptr;	upPNG_ptr++;
				upCol[2] = *upPNG_ptr;	upPNG_ptr++;
				upCol[3] = *upPNG_ptr;	upPNG_ptr++;

				peath[0] = this->peath_algorythm(prevCol[0], upCol[0], upLeftCol[0]);
				peath[1] = this->peath_algorythm(prevCol[1], upCol[1], upLeftCol[1]);
				peath[2] = this->peath_algorythm(prevCol[2], upCol[2], upLeftCol[2]);
				peath[3] = this->peath_algorythm(prevCol[3], upCol[3], upLeftCol[3]);
			
				tempCol[0] = *FiltPNG_dataPtr; FiltPNG_dataPtr++;
				tempCol[1] = *FiltPNG_dataPtr; FiltPNG_dataPtr++;
				tempCol[2] = *FiltPNG_dataPtr; FiltPNG_dataPtr++;
				tempCol[3] = *FiltPNG_dataPtr; FiltPNG_dataPtr++;

				*PNG_dataPtr = (unsigned char)((tempCol[0] + peath[0]) % 256);
				prevCol[0] = *PNG_dataPtr;
				upLeftCol[0] = upCol[0];
				PNG_dataPtr++;

				*PNG_dataPtr = (unsigned char)((tempCol[1] + peath[1]) % 256);
				prevCol[1] = *PNG_dataPtr;
				upLeftCol[1] = upCol[1];
				PNG_dataPtr++;

				*PNG_dataPtr = (unsigned char)((tempCol[2] + peath[2]) % 256);
				prevCol[2] = *PNG_dataPtr;
				upLeftCol[2] = upCol[2];
				PNG_dataPtr++;

				*PNG_dataPtr = (unsigned char)((tempCol[3] + peath[3]) % 256);
				prevCol[3] = *PNG_dataPtr;
				upLeftCol[3] = upCol[3];
				PNG_dataPtr++;
			}

			break;
		} // switch

	}

	
	//*********************************************************************

	// restore PNG pointer
	PNG_dataPtr = restorePNG;


	// free memory
	FiltPNG_dataPtr = restore_FiltPNG;
	free(FiltPNG_dataPtr);
	FiltPNG_dataPtr = NULL;
	
	free(IDAT_ptr);
	


	return yn;
}





bool png24LoaderClass::get_compressed_data_from_IDAT(unsigned char* idatPtr, long length)
{
	//std::cout << "\n*** GET COMPRESSED DATA ***" << std::endl;


	unsigned char* idat_Ptr = idatPtr;

	// IDAT data contens ( first byte )
	// [7][6][5][4][3][2][1][0].....
	// [   CINFO  ][    CM    ]

	// get compression method and flag ( 1byte )
	uint8_t CMF;
	memcpy(&CMF, idat_Ptr, 1);
	idat_Ptr += 1;


	uint8_t CM = CMF & (8 + 4 + 2 + 1); //00001111

	// always 8
	if (CM == 8)
	{
		//std::cout << " -CM [8] ok." << std::endl;
	}
	else
	{
		std::cout << " -CM error... return" << std::endl;
		return false;
	}


	// get deflate window size
	uint8_t CINFO = (CMF >> 4) & (8 + 4 + 2 + 1);//00001111
	uint32_t window_size = 1;

	window_size <<= (CINFO + 8);

	//std::cout << " -deflate window size : " << window_size << std::endl;


	// ***************************************************

	// IDAT data contens ( second byte )

	// ...[7][6][5][4][3][2][1][0].....
	// ...[FLEV][D][  CHECK BIT  ]


	// get FLG
	uint8_t FLG;
	memcpy(&FLG, idat_Ptr, 1);
	idat_Ptr += 1;

	// skip checksum
	FLG >>= 5;

	// check DICTIONARY
	int FDICT = FLG & (1); // 00000001

	//std::cout << " -FDICT : " << FDICT << std::endl;

	if (FDICT != 0)
	{
		std::cout << "ERROR. preset Dictionary is not supported.... return" << std::endl;
		return false;
	}





	// skip DICT ID


	// get actual data length of byte
	long compLength = length - 6; // remove 2bytes header, 4bytes last checksum.
	//std::cout << " -compressed data length : " << compLength << " bytes" << std::endl;




	// get compressed data **************************************
	unsigned char* compData = (unsigned char*)malloc(compLength);
	memcpy(compData, idat_Ptr, compLength);


	// inflate compressed data
	bool yn = this->inflate_compressed_data(compData, compLength);

	if (yn)
	{
		//std::cout << "inflate compressed data SUCCESS" << std::endl;
	}


	// free memory
	free(compData);


	return yn;
}








bool png24LoaderClass::inflate_compressed_data(unsigned char* compPtr, long compLength)
{
	//std::cout << "\n*** INFLATE COMPRESSED DATA ***" << std::endl;

	// prepare for restore PNG pointer
	unsigned char* pngRestore = FiltPNG_dataPtr;


	// pointer for read
	unsigned char* readPtr = compPtr;

	// convert compressed data into num array
	unsigned char* bitChar = (unsigned char*)malloc(compLength * 8);
	unsigned char* restoreChar = bitChar;
	unsigned char* endChar = bitChar + (compLength * 8);

	for (int i = 0; i < compLength; i++)
	{
		uint8_t temp;
		memcpy(&temp, readPtr, 1);
		readPtr++;

		// [7][6][5][4][3][2][1][0], [15][14][13][12][11][10][9][8],........
		for (int k = 0; k < 8; k++)
		{
			*bitChar = (temp & 1);
			bitChar++;
			temp >>= 1;
		}
	}

	bitChar = restoreChar;




	// decode huffman *********************************************
	bool loop = true;
	long COUNTER = 0;

	while (loop)
	{
		// read 3bit
		char head[3];
		head[0] = *bitChar; bitChar++;
		head[1] = *bitChar; bitChar++;
		head[2] = *bitChar; bitChar++;


		// check loop
		if (head[0] == 1)// final block
		{
			//std::cout << "encount final block" << std::endl;
			loop = false;
		}


		if (head[2] == 0 && head[1] == 0)
		{
			// not compressed block
			std::cout << "encount non compressed block,,, not supported.. return" << std::endl;
			free(restoreChar);
			return false;
		}
		else if (head[2] == 0 && head[1] == 1)
		{
			// fixed huffman block
			//std::cout << "FIXED HUFFMAN BLOCK" << std::endl;
			bitChar = this->decode_fixed_huffman(bitChar);

			if (loop == false)
			{
				break;
			}
		}
		else if (head[2] == 1 && head[1] == 0)
		{
			// custom huffman block
			//std::cout << "CUSTOM HUFFMAN BLOCK" << std::endl;
			bitChar = this->decode_custom_huffman(bitChar);

			if (loop == false)
			{
				//std::cout << "over end char" << std::endl;
				break;
			}
		}
		else if (head[2] == 1 && head[1] == 1)
		{
			// error..
			//std::cout << "encount error header... return" << std::endl;
			free(restoreChar);
			return false;
		}
	} // while



	// free
	bitChar = restoreChar;
	free(bitChar);


	// restore png pointer
	FiltPNG_dataPtr = pngRestore;



	return true;
}







unsigned char* png24LoaderClass::decode_fixed_huffman(unsigned char* charPtr)
{
	unsigned char* readPtr = charPtr;

	bool loop = true;

	while (loop) // read until find 256
	{
		// read first 7 bit
		unsigned char compareBit[9];
		for (int i = 0; i < 9; i++)
		{
			compareBit[i] = 255;
		}

		for (int i = 0; i < 7; i++)
		{
			compareBit[i] = *readPtr;
			readPtr++;
		}


		// compare
		for (int i = 7; i <= 9; i++)
		{
			int matchCount = this->check_fixed_lit_huffman_identity(compareBit, i);

			if (matchCount <= 255)
			{
				*FiltPNG_dataPtr = (unsigned char)matchCount;
				FiltPNG_dataPtr++;
				break;
			}
			else if (matchCount == 256)
			{
				//std::cout << "end.." << std::endl;
				loop = false;
				break;
			}
			else if (matchCount >= 257 && matchCount < 286)
			{
				// num of extra bit
				int ex_bit_len = LEN_EX_BIT_TABLE[matchCount - 257];
				int same_length = SAMELEN_TABLE[matchCount - 257];

				int ex_len = 0;
				for (int E = 0; E < ex_bit_len; E++)
				{
					ex_len <<= 1;
					ex_len = ex_len | (*readPtr);	readPtr++;
				}

				//**********************************************
				ex_len = this->reverse_bit(ex_len, ex_bit_len);
				same_length = same_length + ex_len;
				//**********************************************


				// read extra 5 bit for dist
				unsigned char inCompare[5];

				inCompare[0] = *readPtr; readPtr++;
				inCompare[1] = *readPtr; readPtr++;
				inCompare[2] = *readPtr; readPtr++;
				inCompare[3] = *readPtr; readPtr++;
				inCompare[4] = *readPtr; readPtr++;

				int matchCount = this->check_dist_huffman_identity(inCompare, 5);

				if (matchCount == 9999) {
					std::cout << "fixd dist doesnt match error..." << std::endl;
					return readPtr;
				}


				int baseLen = DIST_TABLE[matchCount];
				int exLen = DIST_EX_BIT_TABLE[matchCount];

				int exdist_len = 0;
				for (int E = 0; E < exLen; E++)
				{
					exdist_len <<= 1;
					exdist_len = exdist_len | (*readPtr);
					readPtr++;
				}

				exdist_len = this->reverse_bit(exdist_len, exLen);

				int actual_dist = baseLen + exdist_len;



				// set repeat value
				unsigned char* prevPtr = FiltPNG_dataPtr - actual_dist;
				for (int rep = 0; rep < same_length; rep++)
				{
					*FiltPNG_dataPtr = *prevPtr;
					FiltPNG_dataPtr++;
					prevPtr++;
				}
				break;
			}

			// doesnt match
			if (i != 9)
			{
				compareBit[i] = *readPtr;
				readPtr++;
			}

		}// for i <= 9
	}

	return readPtr;
}


int png24LoaderClass::check_fixed_lit_huffman_identity(unsigned char* bitPtr, int bitWidth)
{
	unsigned char bits[9]; // max 9bit
	unsigned char* readP = bitPtr;

	for (int i = 0; i < bitWidth; i++)
	{
		bits[i] = *readP; readP++;
	}

	bool isMatch;
	int matchCount = 9999;

	for (int i = 0; i < 286; i++)
	{
		isMatch = true;

		if (bitWidth == FIX_LIT_LEN[i])
		{
			for (int k = 0; k < bitWidth; k++)
			{
				if (bits[k] != FIX_LIT_HUFFMAN[i][k])
				{
					isMatch = false;
					break;
				}
			}
		}
		else
		{
			isMatch = false;
		}


		if (isMatch == true)
		{
			matchCount = i;
			break;
		}
	}

	return matchCount;
}




int png24LoaderClass::check_fixed_dist_huffman_identity(unsigned char* bitPtr, int bitWidth)
{
	unsigned char bits[5]; // max 5bits
	unsigned char* readP = bitPtr;

	for (int i = 0; i < bitWidth; i++)
	{
		bits[i] = *readP;	readP++;
	}


	bool isMatch;
	int matchCount = 9999;

	for (int i = 0; i < 32; i++)
	{
		isMatch = true;

		for (int k = 0; k < bitWidth; k++) // bitwidth is always 5
		{
			if (bits[k] != FIX_DIST_HUFFMAN[i][k])
			{
				isMatch = false;
				break;
			}
		}

		if (isMatch == true)
		{
			matchCount = i;
			break;
		}

	}

	return matchCount;
}


//************************************************************************************

unsigned char* png24LoaderClass::decode_custom_huffman(unsigned char* charPtr)
{
	// read until find "256"
	unsigned char* readPtr = charPtr;

	// read HLIT ( 5bits )
	unsigned char HLIT = 0;

	HLIT = HLIT | (*readPtr);	readPtr++;	HLIT <<= 1;
	HLIT = HLIT | (*readPtr);	readPtr++;	HLIT <<= 1;
	HLIT = HLIT | (*readPtr);	readPtr++;	HLIT <<= 1;
	HLIT = HLIT | (*readPtr);	readPtr++;	HLIT <<= 1;
	HLIT = HLIT | (*readPtr);	readPtr++;

	HLIT = this->reverse_bit(HLIT, 5);



	//printf(" -HLIT %d\n", HLIT + 257);


	// HDIST ( 5bits )
	unsigned char HDIST = 0;

	HDIST = HDIST | (*readPtr);	readPtr++;	HDIST <<= 1;
	HDIST = HDIST | (*readPtr);	readPtr++;	HDIST <<= 1;
	HDIST = HDIST | (*readPtr);	readPtr++;	HDIST <<= 1;
	HDIST = HDIST | (*readPtr);	readPtr++;	HDIST <<= 1;
	HDIST = HDIST | (*readPtr);	readPtr++;

	HDIST = this->reverse_bit(HDIST, 5);

	//printf(" -HDIST %d\n", HDIST + 1);



	// HCLEN (4byte)
	unsigned char HCLEN = 0;

	HCLEN = HCLEN | (*readPtr);	readPtr++;	HCLEN <<= 1;
	HCLEN = HCLEN | (*readPtr);	readPtr++;	HCLEN <<= 1;
	HCLEN = HCLEN | (*readPtr);	readPtr++;	HCLEN <<= 1;
	HCLEN = HCLEN | (*readPtr);	readPtr++;

	HCLEN = this->reverse_bit(HCLEN, 4);

	//printf(" -HCLEN %d\n", HCLEN + 4);



	// read huffman of huffman ( read (HCLEN+4)*3 bits )
	int HC_IDX[19] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
	//unsigned char hclen[19];
	// init
	for (int i = 0; i < 19; i++)
	{
		//hclen[i] = 255;
		HC_LEN[i] = 255;
	}

	// get length table
	for (int i = 0; i < (HCLEN + 4); i++)
	{
		unsigned int TEMP = 0;
		TEMP = TEMP | (*readPtr); readPtr++;	TEMP <<= 1;
		TEMP = TEMP | (*readPtr); readPtr++;	TEMP <<= 1;
		TEMP = TEMP | (*readPtr); readPtr++;

		TEMP = this->reverse_bit(TEMP, 3);

		HC_LEN[ HC_IDX[i] ] = TEMP;
	}


	// check minimum length ( other than 0 )
	int MIN = 255;
	for (int i = 0; i < 19; i++)
	{
		if (HC_LEN[i] < MIN && HC_LEN[i] != 0)
		{
			MIN = HC_LEN[i];
		}
	}


	// make huffman from length table
	// start from minimum length -> length 7
	//unsigned char clen_huf[19];

	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			HC_HUFFMAN[i][j] = 255;
		}
	}

	unsigned char base = 0;

	for (int len = MIN; len <= 7; len++) // max 7bit width
	{

		base <<= 1; // 110 -> 1100 etc..

		for (int i = 0; i < 19; i++)
		{
			if (HC_LEN[i] == len)
			{
				int cop = base;
				for (int k = len - 1; k >= 0; k--)
				{
					HC_HUFFMAN[i][k] = cop & 1;
					cop >>= 1;
				}

				base++;
			}
		}

	}


	// create lit length table ( of HLIT + 257 )
	int HLIT_COUNTER = 0;
	bool loop = true;

	for (int i = 0; i < 286; i++)
	{
		LIT_LEN[i] = 255;
	}


	while (loop)
	{
		//unsigned char compareBit = 0;
		unsigned char compareBits[7];
		for (int i = 0; i < 7; i++)
		{
			compareBits[i] = 255;
		}

		//first MIN bit
		for (int i = 0; i < MIN; i++)
		{
			compareBits[i] = (*readPtr);
			readPtr++;
		}


		// check ( based on langrenth method )
		for (int i = MIN; i <= 7; i++)
		{
			int matchCount = this->check_hc_huffman_identity(compareBits, i);

			if (matchCount <= 15)
			{
				LIT_LEN[ HLIT_COUNTER ] = matchCount;
				HLIT_COUNTER++;
				break;
			}
			else if (matchCount == 16)
			{
				// read extra 2bit
				int ex_repeat = 0;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;

				ex_repeat = this->reverse_bit(ex_repeat, 2);


				// get actual repeat count
				int act_repeat = 3 + ex_repeat;

				// get prev value
				int prevVal = LIT_LEN[HLIT_COUNTER - 1];

				// set value
				for (int R = 0; R < act_repeat; R++)
				{
					LIT_LEN[HLIT_COUNTER] = prevVal;
					HLIT_COUNTER++;
				}

				break;
			}
			else if (matchCount == 17) // repeat zero 3 - 10
			{
				// read extra 3bit
				int ex_repeat = 0;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				
				ex_repeat = this->reverse_bit(ex_repeat, 3);

				// get actual repeat
				int act_repeat = 3 + ex_repeat;

				// set zero
				for (int R = 0; R < act_repeat; R++)
				{
					LIT_LEN[HLIT_COUNTER] = 0;
					HLIT_COUNTER++;
				}

				break;
			}
			else if (matchCount == 18) // repeat zero 11-138
			{
				// read extra 7bit
				int ex_repeat = 0;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				
				ex_repeat = this->reverse_bit(ex_repeat, 7);

				// get actural repeat count
				int act_repeat = 11 + ex_repeat;

				// set zero
				for (int R = 0; R < act_repeat; R++)
				{
					LIT_LEN[HLIT_COUNTER] = 0;
					HLIT_COUNTER++;
				}

				break;
			}// if match

			// doesnt match
			if (i != 7)
			{
				compareBits[i] = (*readPtr);
				readPtr++;
			}
			else
			{
				std::cout << "hc doesnt match. error...." << std::endl;
			}
		}// for i <= 7

		if (HLIT_COUNTER >= (HLIT + 257))
		{
			loop = false;
			break;
		}
	} // while loop


	


	// get HDIST huffman table length
	int HDIST_COUNTER = 0;
	loop = true;

	for (int i = 0; i < 32; i++)
	{
		DIST_LEN[i] = 255;
	}

	


	while (loop)
	{
		unsigned char compareBits[7];
		for (int i = 0; i < 7; i++)
		{
			compareBits[i] = 255;
		}

		// first min bit
		for (int k = 0; k < MIN; k++)
		{
			compareBits[k] = (*readPtr);	readPtr++;
		}

		for (int i = MIN; i <= 7; i++)
		{
			int matchCount = this->check_hc_huffman_identity(compareBits, i);

			if (matchCount <= 15)
			{
				DIST_LEN[HDIST_COUNTER] = matchCount;
				HDIST_COUNTER++;
				break;
			}
			else if (matchCount == 16)
			{
				// read extra 2bit
				int ex_repeat = 0;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;

				ex_repeat = this->reverse_bit(ex_repeat, 2);

				// get actual repeat
				int act_repeat = 3 + ex_repeat;

				// get prev value
				int prevValue = DIST_LEN[HDIST_COUNTER - 1];

				// repeat
				for (int R = 0; R < act_repeat; R++)
				{
					DIST_LEN[HDIST_COUNTER] = prevValue;
					HDIST_COUNTER++;
				}

				break;
			}
			else if (matchCount == 17)
			{
				// read extra 3bit
				int ex_repeat = 0;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				
				ex_repeat = this->reverse_bit(ex_repeat, 3);

				// get actual repeat
				int act_repeat = 3 + ex_repeat;

				// set zero
				for (int R = 0; R < act_repeat; R++)
				{
					DIST_LEN[HDIST_COUNTER] = 0;
					HDIST_COUNTER++;
				}

				break;
			}
			else if (matchCount == 18)
			{
				// read extra 7bits
				int ex_repeat = 0;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				ex_repeat <<= 1;
				ex_repeat = ex_repeat | (*readPtr);	readPtr++;
				
				ex_repeat = this->reverse_bit(ex_repeat, 7);

				// get actual repeat count
				int act_repeat = 11 + ex_repeat;

				// set zero
				for (int R = 0; R < act_repeat; R++)
				{
					DIST_LEN[HDIST_COUNTER] = 0;
					HDIST_COUNTER++;
				}
				break;
			}// match

			// doesnt match
			if (i != 7)
			{
				compareBits[i] = (*readPtr);
				readPtr++;
			}
			else
			{
				std::cout << "hcdist doesnt match. error...." << std::endl;
			}
		}// for i <= 7

		if (HDIST_COUNTER >= (HDIST + 1))
		{
			loop = false;
			break;
		}
	}// while loop



	/*
	FILE* debugFile;
	fopen_s(&debugFile, "lit_dist_len_2.txt", "w");

	for (int i = 0; i < 286; i++)
	{
		fprintf_s(debugFile, "CUSTOM_LIT_LEN[%d] : %d\n", i, LIT_LEN[i]);
	}

	fprintf_s(debugFile, "\n DIST LEN \n");
	for (int i = 0; i < 32; i++)
	{
		fprintf_s(debugFile, "CUSTOM_DIST_LEN[%d] : %d\n", i, DIST_LEN[i]);
	}

	fclose(debugFile);
	*/



	// create lit-huffman & dist-huffman

	// find minimum length of LIT HUFFMAN ********************************************
	unsigned char MIN_LIT = 255;


	for (int i = 0; i < (HLIT + 257); i++)
	{
		if (LIT_LEN[i] < MIN_LIT && LIT_LEN[i] != 0)
		{
			MIN_LIT = LIT_LEN[i];
		}
	}

	//////////////////// init HUFFMAN table
	for (int i = 0; i < 286; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			LIT_HUFFMAN[i][j] = 255;
		}
	}

	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			DIST_HUFFMAN[i][j] = 255;
		}
	}



	//unsigned int lit_huf_table[286]; // 0-255(value), 256(terminate), 257-285(same)
	//unsigned char lit_huffman[286][15];
	unsigned int base16 = 0;

	////////////////

	for (int len = MIN_LIT; len <= 15; len++)
	{
		base16 <<= 1; // 110 -> 1100 etc..

		for (int i = 0; i < 286; i++)
		{
			if (LIT_LEN[i] == len)
			{
				int cp = base16;
				for (int k = len-1; k >= 0; k--)
				{
					LIT_HUFFMAN[i][k] = cp & 1;
					cp >>= 1;
				}

				base16++;
			}
		}
	}


	

	// find minimum length of DIST HUFFMAN ***************************************
	unsigned char MIN_DIST = 255;

	for (int i = 0; i < (HDIST + 1); i++)
	{
		if (DIST_LEN[i] < MIN_DIST && DIST_LEN[i] != 0)
		{
			MIN_DIST = DIST_LEN[i];
		}
	}


	base16 = 0;
	for (int len = MIN_DIST; len <= 15; len++)
	{
		base16 <<= 1;

		for (int i = 0; i < 32; i++)
		{
			if (DIST_LEN[i] == len)
			{
				int cp = base16;
				for (int k = len - 1; k >= 0; k--)
				{
					DIST_HUFFMAN[i][k] = cp & 1;
					cp >>= 1;
				}
				base16++;
			}
		}
	}

	/*
	FILE* debugFile;
	fopen_s(&debugFile, "lit_dist_huffman2.txt", "w");

	for (int i = 0; i < 286; i++)
	{
		fprintf_s(debugFile, "bit %d : ", i);
		for (int j = 0; j < LIT_LEN[i]; j++)
		{
			fprintf_s(debugFile, "%d", LIT_HUFFMAN[i][j]);
		}
		fprintf_s(debugFile, "\n");
	}

	fprintf_s(debugFile, "\n DIST HUFFMAN TABLE \n");
	for (int i = 0; i < 32; i++)
	{
		fprintf_s(debugFile, "bit %d : ", i);
		for (int j = 0; j < DIST_LEN[i]; j++)
		{
			fprintf_s(debugFile, "%d", DIST_HUFFMAN[i][j]);
		}
		fprintf_s(debugFile, "\n");
	}

	fclose(debugFile);
	*/
	

	//*******************************************************************************
	// now i got lit_huffman[286][15 & dist_huffman[30][15]
	// decode rest of data until "256" is found.
	//********************************************************************************


	loop = true;

	while (loop) // loop until find "256"
	{
		unsigned char compareBit[15];
		for (int i = 0; i < 15; i++)
		{
			compareBit[i] = 255;
		}

		// get first MIN_LIT bit
		for (int i = 0; i < MIN_LIT; i++)
		{
			compareBit[i] = (*readPtr);
			readPtr++;
		}



		for (int i = MIN_LIT; i <= 15; i++)
		{
			// check match
			int match_count = this->check_lit_huffman_identity(compareBit, i);

			if (match_count <= 255)
			{
				// set value
				*FiltPNG_dataPtr = (unsigned char)match_count;
				FiltPNG_dataPtr++;
				break;
			}
			else if (match_count == 256)
			{
				//end
				//std::cout << "end" << std::endl;
				loop = false;
				break;
			}
			else if (match_count > 256 && match_count < 286 )
			{
				// same repeat
				int same_len = SAMELEN_TABLE[match_count - 257];
				int exbit_len = LEN_EX_BIT_TABLE[match_count - 257];

				// read extra bit
				int ex_len = 0;
				for (int EX = 0; EX < exbit_len; EX++)
				{
					ex_len <<= 1;
					ex_len = ex_len | (*readPtr);
					readPtr++;
				}
				ex_len = this->reverse_bit(ex_len, exbit_len);

				int actual_same_length = same_len + ex_len;

				//************************************************************************************
				// calculate distance

				// init
				unsigned char inCompare[15];

				for (int inC = 0; inC < 15; inC++)
				{
					inCompare[i] = 255;
				}

				// set first DIST_MIN bit
				for (int inC = 0; inC < MIN_DIST; inC++)
				{
					inCompare[inC] = (*readPtr);
					readPtr++;
				}


				for (int inC = MIN_DIST; inC <= 15; inC++)
				{
					int return_dist = this->check_dist_huffman_identity(inCompare, inC);

					if (return_dist < 30)
					{
						int base_dist = DIST_TABLE[return_dist];
						int ex_distbit_len = DIST_EX_BIT_TABLE[return_dist];

						// read extra bit
						int ex_dist = 0;
						for (int ED = 0; ED < ex_distbit_len; ED++)
						{
							ex_dist <<= 1;
							ex_dist = ex_dist | (*readPtr);
							readPtr++;
						}

						ex_dist = this->reverse_bit(ex_dist, ex_distbit_len);

						int actual_distance = base_dist + ex_dist;


						// read same data into FiltPNG_dataPtr( actual same length & actual distance )
						unsigned char* prevPtr = FiltPNG_dataPtr - actual_distance;
						for (int rep = 0; rep < actual_same_length; rep++)
						{
							*FiltPNG_dataPtr = *prevPtr;
							FiltPNG_dataPtr++;
							prevPtr++;
						}

						break;
					}

					// doesnt match
					if (inC != 15)
					{
						inCompare[inC] = (*readPtr);
						readPtr++;
					}
					else
					{
						std::cout << "dist doesnt match. error...." << std::endl;
					}
				} // for inC <= 15

				//************************************************************************************


				break;
			} // if 257 - 285

			// if doesnt match
			if (i != 15) // not last
			{
				compareBit[i] = *readPtr;
				readPtr++;
			}
			else
			{
				std::cout << "lit doesnt match. error...." << std::endl;
			}
		}


	} // while loop



	return readPtr; // read until this pointer

	

}// end




unsigned int png24LoaderClass::reverse_bit(unsigned int num, int bitcount)
{	
	unsigned int retVal = 0;
	unsigned int inVal = num;

	//printf("%d : ", num);
	for (int i = 0; i < bitcount; i++)
	{

		unsigned int bit = inVal & 1;
		inVal >>= 1;

		retVal <<= 1;
		retVal = retVal | bit;


	}
	//printf("\n");
	//printf("in %d ret %d\n", num, retVal);

	return retVal;
}



int png24LoaderClass::check_hc_huffman_identity(unsigned char* bitPtr, int bitWidth)
{
	unsigned char bits[7];
	unsigned char* readP = bitPtr;

	// read compare bits
	for (int i = 0; i < bitWidth ; i++)
	{
		bits[i] = *readP;	readP++;
	}

	// compare
	bool isMatch;
	int matchCount = 9999;
	for (int k = 0; k < 19; k++)
	{
		isMatch = true;

		if (HC_LEN[k] == bitWidth)
		{
			for (int i = 0; i < bitWidth; i++)
			{
				if (bits[i] != HC_HUFFMAN[k][i])
				{
					isMatch = false;
					break;
				}
			}
		}
		else
		{
			isMatch = false;
		}


		if (isMatch == true)
		{
			matchCount = k;
			break;
		}
	}


	return matchCount;
}


int png24LoaderClass::check_lit_huffman_identity(unsigned char* bitPtr, int bitWidth)
{
	unsigned char bits[15];
	unsigned char* readP = bitPtr;

	// read compare bit
	for (int i = 0; i < bitWidth; i++)
	{
		bits[i] = *readP;	readP++;
	}


	// compare
	bool isMatch;
	int matchCount = 9999;
	for (int k = 0; k < 286; k++)
	{
		isMatch = true;
		
		if (LIT_LEN[k] == bitWidth)
		{
			for (int i = 0; i < bitWidth; i++)
			{
				if (bits[i] != LIT_HUFFMAN[k][i])
				{
					isMatch = false;
					break;
				}
			}
		}
		else
		{
			isMatch = false;
		}


		if (isMatch == true)
		{
			matchCount = k;
			break;
		}
	}

	return matchCount;
}



int png24LoaderClass::check_dist_huffman_identity(unsigned char* bitPtr, int bitWidth)
{
	unsigned char bits[15];
	unsigned char* readP = bitPtr;

	// read compare bit
	for (int i = 0; i < bitWidth; i++)
	{
		bits[i] = *readP;
		readP++;
	}


	// compare
	bool isMatch;
	int matchCount = 9999;

	for (int k = 0; k < 32; k++)
	{
		isMatch = true;

		if (DIST_LEN[k] == bitWidth)
		{
			for (int i = 0; i < bitWidth; i++)
			{
				if (bits[i] != DIST_HUFFMAN[k][i])
				{
					isMatch = false;
					break;
				}
			}
		}
		else
		{
			isMatch = false;
		}

		if (isMatch == true)
		{
			matchCount = k;
			break;
		}
	}

	return matchCount;
}