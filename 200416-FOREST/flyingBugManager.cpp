#include "stdafx.h"
#include "flyingBugManager.h"


flyingBugManager::flyingBugManager()
{
	printf("flyingBugManager init\n");

	numVertex = BUG_DIVISION * 6;

	for (int i = 0; i < BUG_DIVISION; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			bug_v[i][j][0] = 0.0;
			bug_v[i][j][1] = 0.0;
			bug_v[i][j][2] = 0.0;
			bug_v[i][j][3] = 1.0;

			bug_t[i][j][0] = 0.0;
			bug_t[i][j][1] = 0.0;
			bug_t[i][j][2] = 1.0; // used for aux
			bug_t[i][j][3] = 0.0; // aux
		}
	}

}


flyingBugManager::~flyingBugManager()
{
}




void flyingBugManager::bug_loop()
{
	for (int i = 0; i < BUG_DIVISION; i++)
	{
		int t_idx = (int)bug_count[i];

		float tunit = 1.0 / 12.0;

		if (t_idx >= 0 && t_idx < 12)
		{
			// texture
			bug_t[i][0][0] = bug_t[i][1][0] = tunit * t_idx;
			bug_t[i][0][1] = bug_t[i][1][1] = 0.0;

			bug_t[i][2][0] = tunit * t_idx;
			bug_t[i][2][1] = 1.0;

			bug_t[i][3][0] = tunit * (t_idx+1);
			bug_t[i][3][1] = 0.0;

			bug_t[i][4][0] = bug_t[i][5][0] = tunit * (t_idx + 1);
			bug_t[i][4][1] = bug_t[i][5][1] = 1.0;
		}
		else
		{
			bug_t[i][0][0] = bug_t[i][1][0] = 0.0;
			bug_t[i][0][1] = bug_t[i][1][1] = 0.0;

			bug_t[i][2][0] = 0.0;
			bug_t[i][2][1] = 0.0;

			bug_t[i][3][0] = 0.0;
			bug_t[i][3][1] = 0.0;

			bug_t[i][4][0] = bug_t[i][5][0] = 0.0;
			bug_t[i][4][1] = bug_t[i][5][1] = 0.0;
		}


		if (bug_count[i] < 20.0)
		{
			bug_count[i] += 0.5;
		}
	}
}


void flyingBugManager::start_bug(float start_x, float start_y, float end_x, float end_y)
{

	unit_Y[0] = (end_x - start_x)*0.5;
	unit_Y[1] = (end_y - start_y)*0.5;
	unit_Y[0] /= (float)(BUG_DIVISION - 1);
	unit_Y[1] /= (float)(BUG_DIVISION - 1);

	unit_X[0] = cosf(1.57079)*unit_Y[0] - sinf(1.57079)*unit_Y[1];
	unit_X[1] = sinf(1.57079)*unit_Y[0] + cosf(1.57079)*unit_Y[1];
	unit_X[0] *= 450.0 / 270.0;
	unit_X[1] *= 450.0 / 270.0;

	float center[2];

	for (int i = 0; i < BUG_DIVISION; i++)
	{
		bug_count[i] = (float)(-i);

		// vertex
		center[0] = start_x + (unit_Y[0]*2.0*i);
		center[1] = start_y + (unit_Y[1]*2.0*i);

		bug_v[i][0][0] = center[0] + (unit_X[0] + unit_Y[0]);
		bug_v[i][0][1] = center[1] + (unit_X[1] + unit_Y[1]);
		bug_v[i][1][0] = bug_v[i][0][0];
		bug_v[i][1][1] = bug_v[i][0][1];

		bug_v[i][2][0] = center[0] +(unit_X[0] - unit_Y[0]);
		bug_v[i][2][1] = center[1] +(unit_X[1] - unit_Y[0]);

		bug_v[i][3][0] = center[0] + ( -unit_X[0] + unit_Y[0] );
		bug_v[i][3][1] = center[1] + ( -unit_X[1] + unit_Y[1] );

		bug_v[i][4][0] = center[0] + ( -unit_X[0] - unit_Y[0] );
		bug_v[i][4][1] = center[1] + ( -unit_X[1] - unit_Y[1] );
		bug_v[i][5][0] = bug_v[i][4][0];
		bug_v[i][5][1] = bug_v[i][4][1];

		// texture
		bug_t[i][0][0] = bug_t[i][1][0] = 0.0;
		bug_t[i][0][1] = bug_t[i][1][1] = 0.0;

		bug_t[i][2][0] = 0.0;
		bug_t[i][2][1] = 0.0;

		bug_t[i][3][0] = 0.0;
		bug_t[i][3][1] = 0.0;

		bug_t[i][4][0] = bug_t[i][5][0] = 0.0;
		bug_t[i][4][1] = bug_t[i][5][1] = 0.0;
	}
}


float* flyingBugManager::get_bug_v_pointer()
{
	float* vPtr = &bug_v[0][0][0];
	return vPtr;
}

float* flyingBugManager::get_bug_t_pointer()
{
	float* tPtr = &bug_t[0][0][0];
	return tPtr;
}
