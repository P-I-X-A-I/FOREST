#include "stdafx.h"
#include "windParticleManager.h"


windParticleManager::windParticleManager()
{
	printf("\n****** windParticleManager init *********\n");

	bufSize = NUM_PARTICLE * NUM_TAIL * 4;

	for (int i = 0; i < NUM_PARTICLE; i++)
	{
		for (int j = 0; j < NUM_TAIL; j++)
		{
			pVert[i][j][0] = (float)(rand() % 2000 - 1000)*0.001;
			pVert[i][j][1] = (float)(rand() % 2000 - 1000)*0.001;
			pVert[i][j][2] = 0.0;
			pVert[i][j][3] = 1.0;

			pCol[i][j][0] = 1.0;
			pCol[i][j][1] = 1.0;
			pCol[i][j][2] = 1.0;
			pCol[i][j][3] = (rand() % 1000)*0.001;

			pAux[i][j][0] = 1.0;
			pAux[i][j][1] = 0.0;
			pAux[i][j][2] = 0.0;
			pAux[i][j][3] = (rand()%314)*0.01; // use for radian  of inner wind force

		}
		pSpeed[i][0] = (float)(rand() % 2000 - 1000)*0.00001;
		pSpeed[i][1] = (float)(rand() % 2000 - 1000)*0.00001;
	}


	wForce[0] = 0.0;
	wForce[1] = 0.0001;


	num_particle = NUM_PARTICLE * NUM_TAIL;

	is_kill_all = false;

	wind_speed = 0.0;
	wind_degree = 0;
}


windParticleManager::~windParticleManager()
{
}



void windParticleManager::windParticle_loop()
{

	// convert wind degree to screen space
	int wind_out_degree = (wind_degree + 180);
	if (wind_out_degree > 360)
	{
		wind_out_degree -= 360;
	}

	float wind_in_position = (float)(wind_degree - 180) / 180.0 * (9600.0 / 1200.0);
	float wind_out_position = (float)(wind_out_degree - 180) / 180.0 * (9600.0 / 1200.0);

	// convert wind speed
	float wind_speed_coef = sqrtf(wind_speed);



	for (int i = 0; i < NUM_PARTICLE; i++)
	{
		// calc force
		float eachForce[2];
		int C_ID, N_ID;
		
		C_ID = i;
		N_ID = i + 1;

		if (N_ID == NUM_PARTICLE) { N_ID = 0; }

		eachForce[0] = pVert[C_ID][0][0] - pVert[N_ID][0][0];
		eachForce[1] = pVert[C_ID][0][1] - pVert[N_ID][0][1];

		// normalize to 0.00001
		float tempDist = sqrt(eachForce[0] * eachForce[0] + eachForce[1] * eachForce[1]);
		if (tempDist != 0.0)
		{
			eachForce[0] /= (tempDist)*30000.0;
			eachForce[1] /= (tempDist)*30000.0;
		}



		// wind force of forest
		float forest_in_F = pVert[C_ID][0][0] - wind_in_position;
		float forest_out_F = wind_out_position - pVert[C_ID][0][0];
		float forest_XF;

		float dist_to_inPoint = fabs( wind_in_position - pVert[C_ID][0][0]);
		float dist_to_outPoint = fabs( wind_out_position - pVert[C_ID][0][0]);

		if (dist_to_inPoint != 0.0)
		{
			forest_in_F /= dist_to_inPoint;
		}
		else
		{
			forest_in_F = 0.0;
		}

		if (dist_to_outPoint != 0.0)
		{
			forest_out_F /= dist_to_outPoint;
		}
		else
		{
			forest_out_F = 0.0;
		}


		forest_in_F *= (0.0001*wind_speed_coef);
		forest_out_F *= (0.0001*wind_speed_coef);


		if (dist_to_inPoint > dist_to_outPoint)
		{
			forest_XF = forest_out_F;
		}
		else
		{
			forest_XF = forest_in_F;
		}



		// inner wind force
		wForce[1] = sinf(pAux[C_ID][0][3])*0.0001;
		pAux[C_ID][0][3] += 0.001;


		// calcrate speed
		pSpeed[i][0] = pSpeed[i][0] * 0.93 + wForce[0] + eachForce[0] + forest_XF;
		pSpeed[i][1] = pSpeed[i][1] * 0.93 + wForce[1] + eachForce[1];

		// tail position
		for (int j = NUM_TAIL - 1; j > 0; j--)
		{
			pVert[i][j][0] = pVert[i][j-1][0];
			pVert[i][j][1] = pVert[i][j-1][1];

			pCol[i][j][3] = pCol[i][j - 1][3];
		}

		// add to position
		pVert[i][0][0] += pSpeed[i][0];
		pVert[i][0][1] += pSpeed[i][1];



		// reduce alpha ( as life )
		pCol[i][0][3] *= 0.98;

		// reset position
		if (pCol[i][0][3] < 0.001 && is_kill_all == false)
		{
			pVert[i][0][0] = (float)(rand() % 2000 - 1000) * 0.001 * screen_ratio;
			pVert[i][0][1] = (float)(rand() % 2000 - 1000) * 0.001;

			pVert[i][1][0] = pVert[i][2][0] = pVert[i][0][0];
			pVert[i][1][1] = pVert[i][2][1] = pVert[i][0][1];


			pSpeed[i][0] = (float)(rand() % 2000 - 1000)*0.000005;
			pSpeed[i][1] = (float)(rand() % 2000 - 1000)*0.000005;

			pCol[i][0][3] = pCol[i][1][3] = pCol[i][2][3] = (float)(rand()%100) * 0.01;

			pAux[i][0][3] = (rand() % 628)*0.01;
		}
	}
}


float* windParticleManager::getVertPointer()
{
	return &pVert[0][0][0];
}

float* windParticleManager::getColorPointer()
{
	return &pCol[0][0][0];
}

float* windParticleManager::getAuxPointer()
{
	return &pAux[0][0][0];
}
