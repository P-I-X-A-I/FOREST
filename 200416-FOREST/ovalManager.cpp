#include "stdafx.h"
#include "ovalManager.h"


ovalManager::ovalManager()
{
	printf("init ovalManager.\n");

	// init variables

	num_vertex = CIRCLE_DIVISION;

	OVAL_CENTER[0] = -100.0;
	OVAL_CENTER[1] = -100.0;

	OVAL_CENTER_VELOCITY[0] = 0.0;
	OVAL_CENTER_VELOCITY[1] = 0.0;

	OVAL_SIZE = 0.3;
	
	EDGE_LENGTH = (OVAL_SIZE*2.0) * 3.14159265 / (float)(CIRCLE_DIVISION - 2);

	for (int i = 0; i < CIRCLE_DIVISION; i++)
	{
		OVAL_VELOCITY[i][0] = 0.0;
		OVAL_VELOCITY[i][1] = 0.0;

		OVAL_V[i][0] = -100.0; //x
		OVAL_V[i][1] = -100.0; //y
		OVAL_V[i][2] = 0.0; //z
		OVAL_V[i][3] = 1.0; //w

		OVAL_C[i][0] = 1.0; //r
		OVAL_C[i][1] = 1.0; //g
		OVAL_C[i][2] = 1.0; //b
		OVAL_C[i][3] = 1.0; //a

		OVAL_AUX[i][0] = 0.0;
		OVAL_AUX[i][1] = 0.0;
		OVAL_AUX[i][2] = 0.0;
		OVAL_AUX[i][3] = 0.0;
	}

	for (int i = 0; i < 8; i++)
	{
		WAVE_CYCLE[i] = (rand() % 628)*0.01;
		WAVE_CYCLE_add[i] = (rand() % 200 - 100) * 0.00002;
	}
}


ovalManager::~ovalManager()
{
}



void ovalManager::set_new_oval( float size )
{

	// reset wave cycle
	for (int i = 0; i < 8; i++)
	{
		WAVE_CYCLE[i] = (rand() % 628)*0.01;
		WAVE_CYCLE_add[i] = (rand() % 200 - 100) * 0.00002;
	}
	float CENTER_DIST[2];
	CENTER_DIST[0] = sin(WAVE_CYCLE[0])*cos(WAVE_CYCLE[1])*sin(WAVE_CYCLE[2]) * 5.0;
	CENTER_DIST[1] = cos(WAVE_CYCLE[3])*sin(WAVE_CYCLE[4])*cos(WAVE_CYCLE[5]) * 5.0;
	
	// reset center
	OVAL_CENTER[0] = CENTER_DIST[0];
	OVAL_CENTER[1] = CENTER_DIST[1];


	// reset vertex
	OVAL_V[0][0] = OVAL_CENTER[0];
	OVAL_V[0][1] = OVAL_CENTER[1];


	float unit_deg = 360.0 / (float)(CIRCLE_DIVISION - 2);
	float unit_rad = unit_deg * 0.01745329;


	for (int i = 0; i < CIRCLE_DIVISION - 1; i++)
	{
		OVAL_V[i + 1][0] = cos(unit_rad*i) * OVAL_SIZE + OVAL_CENTER[0];
		OVAL_V[i + 1][1] = sin(unit_rad*i) * OVAL_SIZE + OVAL_CENTER[1];
	}


	OVAL_V[CIRCLE_DIVISION - 1][0] = OVAL_V[1][0];
	OVAL_V[CIRCLE_DIVISION - 1][1] = OVAL_V[1][1];


	// reset size
	OVAL_SIZE = size;
	EDGE_LENGTH = (OVAL_SIZE*2.0) * 3.14159265 / (float)(CIRCLE_DIVISION - 2);

}



void ovalManager::update_oval()
{
	for (int i = 0; i < 8; i++)
	{
		WAVE_CYCLE[i] += WAVE_CYCLE_add[i];

		if (WAVE_CYCLE[i] > 6.2831853)
		{
			WAVE_CYCLE[i] -= 6.2831853;
		}
	}


	// set oval center ***********************************************
	float CENTER_DIST[2];
	CENTER_DIST[0] = sin(WAVE_CYCLE[0])*cos(WAVE_CYCLE[1])*sin(WAVE_CYCLE[2]) * 5.0;
	CENTER_DIST[1] = cos(WAVE_CYCLE[3])*sin(WAVE_CYCLE[4])*cos(WAVE_CYCLE[5]) * 5.0;

	if (rand() % 300 == 0)
	{
		for (int i = 0; i < 8; i++)
		{
			WAVE_CYCLE[i] += (rand()%100)*0.001;

			if (WAVE_CYCLE[i] > 6.2831853)
			{
				WAVE_CYCLE[i] -= 6.2831853;
			}
		}
	}

	float CENTER_F[2];
	float CENTER_V_VEC[2];
	CENTER_F[0] = (CENTER_DIST[0] - OVAL_CENTER[0]) * 0.001;
	CENTER_F[1] = (CENTER_DIST[1] - OVAL_CENTER[1]) * 0.001;

	OVAL_CENTER_VELOCITY[0] = OVAL_CENTER_VELOCITY[0] * 0.985 + CENTER_F[0];
	OVAL_CENTER_VELOCITY[1] = OVAL_CENTER_VELOCITY[1] * 0.985 + CENTER_F[1];

	OVAL_CENTER[0] += OVAL_CENTER_VELOCITY[0];
	OVAL_CENTER[1] += OVAL_CENTER_VELOCITY[1];

	// center point velocity vector
	CENTER_V_VEC[0] = OVAL_CENTER_VELOCITY[0];
	CENTER_V_VEC[1] = OVAL_CENTER_VELOCITY[1];

	float cvDist = sqrt( CENTER_V_VEC[0] * CENTER_V_VEC[0] + CENTER_V_VEC[1] * CENTER_V_VEC[1]);
	if (cvDist != 0.0)
	{
		CENTER_V_VEC[0] /= cvDist;
		CENTER_V_VEC[1] /= cvDist;
	}


	// set vertex ******************************************************************


	float unit_deg = 360.0 / (float)(CIRCLE_DIVISION - 2);
	float unit_rad = unit_deg * 0.01745329;
	float V_DIST[2];


	float averageCenter[2] = { 0.0, 0.0 };



	for (int i = 0; i < CIRCLE_DIVISION - 1; i++)
	{


		// power to v distination ******************************
		float radian = unit_rad * i;

		float circleVec[2];
		float dotVal;

		circleVec[0] = cos(radian);
		circleVec[1] = sin(radian);
		
		dotVal = (CENTER_V_VEC[0] * circleVec[0] + CENTER_V_VEC[1] * circleVec[1]); // -1.0. ~ 1.0
		dotVal = max(0.05, dotVal);

		V_DIST[0] = circleVec[0] * OVAL_SIZE + OVAL_CENTER[0];
		V_DIST[1] = circleVec[1] * OVAL_SIZE + OVAL_CENTER[1];


		float VF[2];
		float diff_to_dist[2];
		diff_to_dist[0] = (V_DIST[0] - OVAL_V[i + 1][0]);
		diff_to_dist[1] = (V_DIST[1] - OVAL_V[i + 1][1]);

		VF[0] = diff_to_dist[0] * 0.001 * dotVal;
		VF[1] = diff_to_dist[1] * 0.001 * dotVal;



		// power to center point ***********************
		
	


		float DF[2] = {0.0, 0.0};
		float distToCenter[2];

		distToCenter[0] = OVAL_CENTER[0] - OVAL_V[i + 1][0];
		distToCenter[1] = OVAL_CENTER[1] - OVAL_V[i + 1][1];
		
		float dist = sqrt(distToCenter[0] * distToCenter[0] + distToCenter[1] * distToCenter[1]);

		if (dist < OVAL_SIZE)
		{
			float coef = (OVAL_SIZE - dist)*0.001;
			DF[0] = -distToCenter[0] * coef;
			DF[1] = -distToCenter[1] * coef;
		}
		else
		{
			float coef = (dist - OVAL_SIZE)*0.003;
			DF[0] = distToCenter[0] * coef;
			DF[1] = distToCenter[1] * coef;
		}


		// force to neighborhood
		float PF[2];
		float NF[2];
		int pIDX, nIDX;
		if (i == 0)
		{
			pIDX = CIRCLE_DIVISION - 2;
			nIDX = i + 2;
		}
		else if (i == CIRCLE_DIVISION - 2)
		{
			pIDX = i;
			nIDX = 1;
		}
		else
		{
			pIDX = i;
			nIDX = i + 2;
		}


		PF[0] = OVAL_V[pIDX][0] - OVAL_V[i + 1][0];
		PF[1] = OVAL_V[pIDX][1] - OVAL_V[i + 1][1];
		NF[0] = OVAL_V[nIDX][0] - OVAL_V[i + 1][0];
		NF[1] = OVAL_V[nIDX][1] - OVAL_V[i + 1][1];

		float pDist = sqrt(PF[0] * PF[0] + PF[1] * PF[1]);
		float nDist = sqrt(NF[0] * NF[0] + NF[1] * NF[1]);

		PF[0] = PF[0] * (pDist - EDGE_LENGTH) * 0.03;
		PF[1] = PF[1] * (pDist - EDGE_LENGTH) * 0.03;
		NF[0] = NF[0] * (nDist - EDGE_LENGTH) * 0.03;
		NF[1] = NF[1] * (nDist - EDGE_LENGTH) * 0.03;






		// final force
		float TOTAL_F[2];
		TOTAL_F[0] = DF[0] + VF[0] + PF[0] + NF[0];
		TOTAL_F[1] = DF[1] + VF[1] + PF[1] + NF[1];

		// add force to velocity
		OVAL_VELOCITY[i + 1][0] = OVAL_VELOCITY[i + 1][0] * 0.985 + TOTAL_F[0];
		OVAL_VELOCITY[i + 1][1] = OVAL_VELOCITY[i + 1][1] * 0.985 + TOTAL_F[1];

		// add to vertex
		OVAL_V[i + 1][0] += OVAL_VELOCITY[i + 1][0];
		OVAL_V[i + 1][1] += OVAL_VELOCITY[i + 1][1];


		// set velocity to aux buffer
		OVAL_AUX[i + 1][0] = OVAL_VELOCITY[i + 1][0];
		OVAL_AUX[i + 1][1] = OVAL_VELOCITY[i + 1][1];
		OVAL_AUX[i + 1][2] = diff_to_dist[0]*3.0;
		OVAL_AUX[i + 1][3] = diff_to_dist[1]*3.0;

		// calculate average center
		averageCenter[0] += OVAL_V[i + 1][0];
		averageCenter[1] += OVAL_V[i + 1][1];
	}


	// set center vertex
	OVAL_V[0][0] = averageCenter[0] / (float)(CIRCLE_DIVISION - 2);
	OVAL_V[0][1] = averageCenter[1] / (float)(CIRCLE_DIVISION - 2);


	// set last vertex
	OVAL_V[CIRCLE_DIVISION - 1][0] = OVAL_V[1][0];
	OVAL_V[CIRCLE_DIVISION - 1][1] = OVAL_V[1][1];
}





float* ovalManager::get_oval_v_pointer()
{
	float* vPtr = &OVAL_V[0][0];
	return vPtr;
}

float* ovalManager::get_oval_c_pointer()
{
	float* cPtr = &OVAL_C[0][0];
	return cPtr;
}

float* ovalManager::get_oval_aux_pointer()
{
	float* aPtr = &OVAL_AUX[0][0];
	return aPtr;
}
