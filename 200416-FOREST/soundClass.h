#pragma once

#include "fmod.h"

class soundClass
{
public:
	soundClass();
	~soundClass();


	FMOD_SYSTEM* fmod_system_obj[4];
	FMOD_SOUND* fmod_sound_obj[4][8];
	FMOD_CHANNEL* fmod_channel_obj[4][8];

	float act_volume[4][8];
	float dst_volume[4][8];

	float bigCount;


	void check_sound_loop();

	void play_big();
};

