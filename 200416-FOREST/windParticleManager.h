#pragma once

#define NUM_PARTICLE 400
#define NUM_TAIL 8

class windParticleManager
{
public:
	windParticleManager();
	~windParticleManager();

	int bufSize;
	int num_particle;
	float screen_ratio;
	BOOL is_kill_all;

	float pVert[NUM_PARTICLE][NUM_TAIL][4];
	float pCol[NUM_PARTICLE][NUM_TAIL][4];
	float pAux[NUM_PARTICLE][NUM_TAIL][4];

	float wind_speed;
	int wind_degree;

	float pSpeed[NUM_PARTICLE][2];
	float wForce[2];


	void windParticle_loop();

	float* getVertPointer();
	float* getColorPointer();
	float* getAuxPointer();

};

