#include "stdafx.h"
#include "soundClass.h"


soundClass::soundClass()
{
	printf("soundClass init\n");



	// set variables
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			act_volume[i][j] = 0.0;
			dst_volume[i][j] = 1.0;
		}
	}

	dst_volume[0][0] = 1.0; // air leaf
	dst_volume[0][1] = 1.0; // forest noise
	dst_volume[0][2] = 1.0; // frog noise
	dst_volume[0][3] = 1.0; // synth

	dst_volume[1][0] = 1.0; // air leaf
	dst_volume[1][1] = 1.0; // synth
	dst_volume[1][2] = 1.0; // big

	dst_volume[2][0] = 1.0; // forest noise
	dst_volume[2][1] = 1.0; // frog noise
	dst_volume[2][2] = 0.0;	// swish a

	dst_volume[3][0] = 1.0;	// forest noise
	dst_volume[3][1] = 1.0;	// frog noise
	dst_volume[3][2] = 0.0; // swish b

	bigCount = 0.0;

	// create main system object
	FMOD_RESULT result;

	for (int i = 0; i < 4; i++)
	{
		while (1)
		{
			result = FMOD_System_Create(&fmod_system_obj[i]);
			printf("*");
			Sleep(200);

			if (result == FMOD_OK)
			{
				printf("FMOD system %d create success\n", i);
				break;
			}
		}







		while (1)
		{
			result = FMOD_System_Init(fmod_system_obj[i], 2, FMOD_INIT_NORMAL, 0);
			printf("*");
			Sleep(200);
			if (result == FMOD_OK)
			{
				printf("FMOD system %d Init success\n", i);
				break;
			}
		}
	}// for 4



	// check & set driver to system object

	int numDrivers = 0;
	FMOD_System_GetNumDrivers(fmod_system_obj[0], &numDrivers);
	printf("Fmod num drivers : %d\n", numDrivers);

	for (int i = 0; i < numDrivers; i++)
	{
		FMOD_GUID tempGUID = {0, 0, 0, "aaaaaaa"};

		
		char nameChar[256];
		char copyChar[13];
		FMOD_System_GetDriverInfo(fmod_system_obj[0], i, nameChar, 256, &tempGUID);
		printf("FMOD driver name : %s\n", nameChar);

		for (int t = 0; t < 12; t++)
		{
			copyChar[t] = nameChar[t];
		}
		copyChar[12] = 0; // append NUL

		if (strcmp(copyChar, "Main Out 1-2") == 0)
		{
			printf("*** main out 1.2 ***\n");
			FMOD_System_SetDriver(fmod_system_obj[0], i);
		}
		else if (strcmp(copyChar, "Line Out 3-4") == 0)
		{
			printf("*** line 3.4 ***\n");
			FMOD_System_SetDriver(fmod_system_obj[1], i);
		}
		else if (strcmp(copyChar, "Line Out 5-6") == 0)
		{
			printf("*** line 5.6 ***\n");
			FMOD_System_SetDriver(fmod_system_obj[2], i);
		}
		else if (strcmp(copyChar, "Line Out 7-8") == 0)
		{
			printf("*** line 7.8 ***\n");
			FMOD_System_SetDriver(fmod_system_obj[3], i);
		}

	}


	// channel 1-2
	result = FMOD_System_CreateStream(fmod_system_obj[0], "SOUND/air_leaf.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, NULL, &fmod_sound_obj[0][0]);
	result = FMOD_System_CreateStream(fmod_system_obj[0], "SOUND/forest_noise.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, NULL, &fmod_sound_obj[0][1]);
	result = FMOD_System_CreateStream(fmod_system_obj[0], "SOUND/frog_noise.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, NULL, &fmod_sound_obj[0][2]);
	result = FMOD_System_CreateStream(fmod_system_obj[0], "SOUND/synth.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, NULL, &fmod_sound_obj[0][3]);
	// channel 3-4
	result = FMOD_System_CreateStream(fmod_system_obj[1], "SOUND/air_leaf.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, NULL, &fmod_sound_obj[1][0]);
	result = FMOD_System_CreateStream(fmod_system_obj[1], "SOUND/synth.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, NULL, &fmod_sound_obj[1][1]);
	result = FMOD_System_CreateStream(fmod_system_obj[1], "SOUND/big_size.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, NULL, &fmod_sound_obj[1][2]);
	// channel 5-6
	result = FMOD_System_CreateStream(fmod_system_obj[2], "SOUND/forest_noise.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, NULL, &fmod_sound_obj[2][0]);
	result = FMOD_System_CreateStream(fmod_system_obj[2], "SOUND/frog_noise.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, NULL, &fmod_sound_obj[2][1]);
	result = FMOD_System_CreateStream(fmod_system_obj[2], "SOUND/forest_swish_A.wav", FMOD_DEFAULT, NULL, &fmod_sound_obj[2][2]);
	// channel 7-8
	result = FMOD_System_CreateStream(fmod_system_obj[3], "SOUND/forest_noise.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, NULL, &fmod_sound_obj[3][0]);
	result = FMOD_System_CreateStream(fmod_system_obj[3], "SOUND/frog_noise.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, NULL, &fmod_sound_obj[3][1]);
	result = FMOD_System_CreateStream(fmod_system_obj[3], "SOUND/forest_swish_B.wav", FMOD_DEFAULT, NULL, &fmod_sound_obj[3][2]);





	// play BGM
	result = FMOD_System_PlaySound(fmod_system_obj[0], FMOD_CHANNEL_FREE, fmod_sound_obj[0][0], FALSE, &fmod_channel_obj[0][0]); // main speaker
	result = FMOD_System_PlaySound(fmod_system_obj[0], FMOD_CHANNEL_FREE, fmod_sound_obj[0][1], FALSE, &fmod_channel_obj[0][1]); // main speaker
	result = FMOD_System_PlaySound(fmod_system_obj[0], FMOD_CHANNEL_FREE, fmod_sound_obj[0][2], FALSE, &fmod_channel_obj[0][2]); // main speaker
	result = FMOD_System_PlaySound(fmod_system_obj[0], FMOD_CHANNEL_FREE, fmod_sound_obj[0][3], FALSE, &fmod_channel_obj[0][3]); // main speaker

	result = FMOD_System_PlaySound(fmod_system_obj[1], FMOD_CHANNEL_FREE, fmod_sound_obj[1][0], FALSE, &fmod_channel_obj[1][0]); // woofer
	result = FMOD_System_PlaySound(fmod_system_obj[1], FMOD_CHANNEL_FREE, fmod_sound_obj[1][1], FALSE, &fmod_channel_obj[1][1]); // woofer
	result = FMOD_System_PlaySound(fmod_system_obj[1], FMOD_CHANNEL_FREE, fmod_sound_obj[1][2], FALSE, &fmod_channel_obj[1][1]); // woofer


	result = FMOD_System_PlaySound(fmod_system_obj[2], FMOD_CHANNEL_FREE, fmod_sound_obj[2][0], FALSE, &fmod_channel_obj[2][0]); // rotate a
	result = FMOD_System_PlaySound(fmod_system_obj[2], FMOD_CHANNEL_FREE, fmod_sound_obj[2][1], FALSE, &fmod_channel_obj[2][1]); // rotate a
	result = FMOD_System_PlaySound(fmod_system_obj[2], FMOD_CHANNEL_FREE, fmod_sound_obj[2][2], FALSE, &fmod_channel_obj[2][2]); // rotate a

	
	result = FMOD_System_PlaySound(fmod_system_obj[3], FMOD_CHANNEL_FREE, fmod_sound_obj[3][0], FALSE, &fmod_channel_obj[3][0]); // roatate B
	result = FMOD_System_PlaySound(fmod_system_obj[3], FMOD_CHANNEL_FREE, fmod_sound_obj[3][1], FALSE, &fmod_channel_obj[3][1]); // roatate B
	result = FMOD_System_PlaySound(fmod_system_obj[3], FMOD_CHANNEL_FREE, fmod_sound_obj[3][2], FALSE, &fmod_channel_obj[3][2]); // roatate B

	// set volume
	FMOD_Channel_SetVolume(fmod_channel_obj[0][0], 0.0); // air leaf
	FMOD_Channel_SetVolume(fmod_channel_obj[0][1], 0.0); // forest noise
	FMOD_Channel_SetVolume(fmod_channel_obj[0][2], 0.0); // frog noise
	FMOD_Channel_SetVolume(fmod_channel_obj[0][3], 0.0); // synth


	FMOD_Channel_SetVolume(fmod_channel_obj[1][0], 0.0); // air leaf
	FMOD_Channel_SetVolume(fmod_channel_obj[1][1], 0.0); // synth
	FMOD_Channel_SetVolume(fmod_channel_obj[1][2], 0.0); // big

	FMOD_Channel_SetVolume(fmod_channel_obj[2][0], 0.0); // forest noise
	FMOD_Channel_SetVolume(fmod_channel_obj[2][1], 0.0); // frog noise
	FMOD_Channel_SetVolume(fmod_channel_obj[2][2], 0.0); // swish a

	FMOD_Channel_SetVolume(fmod_channel_obj[3][0], 0.0); // forest noise
	FMOD_Channel_SetVolume(fmod_channel_obj[3][1], 0.0); // frog noise
	FMOD_Channel_SetVolume(fmod_channel_obj[3][2], 0.0); // swish b
}


soundClass::~soundClass()
{

	
}



void soundClass::check_sound_loop()
{

	// big count
	bigCount += (0.0 - bigCount)*0.01;

	// set volume
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			act_volume[i][j] += (dst_volume[i][j] - act_volume[i][j])*0.01;
		}
	}

	FMOD_Channel_SetVolume(fmod_channel_obj[0][0], act_volume[0][0]);
	FMOD_Channel_SetVolume(fmod_channel_obj[0][1], act_volume[0][1]);
	FMOD_Channel_SetVolume(fmod_channel_obj[0][2], act_volume[0][2]);
	FMOD_Channel_SetVolume(fmod_channel_obj[0][3], act_volume[0][3]);

	FMOD_Channel_SetVolume(fmod_channel_obj[1][0], act_volume[1][0]);
	FMOD_Channel_SetVolume(fmod_channel_obj[1][0], act_volume[1][1]);
	FMOD_Channel_SetVolume(fmod_channel_obj[1][1], act_volume[1][2] * sinf(bigCount)*0.7);

	FMOD_Channel_SetVolume(fmod_channel_obj[2][0], act_volume[2][0]);
	FMOD_Channel_SetVolume(fmod_channel_obj[2][1], act_volume[2][1]);
	FMOD_Channel_SetVolume(fmod_channel_obj[2][2], act_volume[2][2]);

	FMOD_Channel_SetVolume(fmod_channel_obj[3][0], act_volume[3][0]);
	FMOD_Channel_SetVolume(fmod_channel_obj[3][1], act_volume[3][1]);
	FMOD_Channel_SetVolume(fmod_channel_obj[3][2], act_volume[3][2]);


	
}




void soundClass::play_big()
{
	bigCount = 3.14159265;
}
