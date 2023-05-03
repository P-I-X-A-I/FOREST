#include "stdafx.h"
#include "lifeGameManager.h"
#include "mainController.h"

lifeGameManager::lifeGameManager()
{
	printf("lifeGameManager init\n");

	// init variables
	SWAP = 0;

	for (int x = 0; x < NUM_GRID_X; x++)
	{
		for (int y = 0; y < NUM_GRID_Y; y++)
		{
			L_ALIVE[0][x][y] = false;
			L_ALIVE[1][x][y] = false;

			L_HOWLONG[x][y] = 0.0;

			L_OVERHEAT[x][y] = false;
		}
	}


	// calculate XY grid center position
	float RATIO = (float)NUM_GRID_X / (float)NUM_GRID_Y;
	
	for (int x = 0; x < NUM_GRID_X; x++)
	{
		for (int y = 0; y < NUM_GRID_Y; y++)
		{
			float pX = (float)x / (float)NUM_GRID_X;
			float pY = (float)y / (float)NUM_GRID_Y;

			pX = (pX*2.0 - 1.0) * RATIO * 1.05;
			pY = (pY*2.0 - 1.0) * 1.05;

			L_XY_POS[x][y][0] = pX;
			L_XY_POS[x][y][1] = pY;

			L_HOWMANY_NEIGHBOR[x][y] = 0;
		}
	}



	ACT_NUM_GRID_X = NUM_GRID_X;
	ACT_NUM_GRID_Y = NUM_GRID_Y;

	sizeRange[0] = 0.01;
	sizeRange[1] = 0.1;

	minimum_active_cell = 300;

	cell_kakuritu = 6;
	cell_anim_speed = 1.0;

	is_kill_all = false;
}


lifeGameManager::~lifeGameManager()
{
}




void lifeGameManager::reset_condition(int W, int H)
{
	ACT_NUM_GRID_X = W;
	ACT_NUM_GRID_Y = H;

	// calculate cell position
	float RATIO = (float)ACT_NUM_GRID_X / (float)ACT_NUM_GRID_Y;


	for (int x = 0; x < ACT_NUM_GRID_X; x++)
	{
		for (int y = 0; y < ACT_NUM_GRID_Y; y++)
		{
			float pX = (float)x / (float)ACT_NUM_GRID_X;
			float pY = (float)y / (float)ACT_NUM_GRID_Y;

			pX = (pX*2.0 - 1.0) * RATIO *1.05;
			pY = (pY*2.0 - 1.0) * 1.05;

			L_XY_POS[x][y][0] = pX;
			L_XY_POS[x][y][1] = pY;
		}
	}
}



void lifeGameManager::set_size_range(float small, float big)
{
	sizeRange[0] = small;
	sizeRange[1] = big;
}


void lifeGameManager::set_minimum_active_cell(int num)
{
	minimum_active_cell = num;
}


void lifeGameManager::set_cell_kakuritu(int val)
{
	cell_kakuritu = val;
}

void lifeGameManager::set_cell_anim_speed(float spe)
{
	cell_anim_speed = spe;
}




void lifeGameManager::loop()
{
	int num_active_cell = 0;

	int CUR, NEX;
	
	if (SWAP == 0)
	{
		CUR = 0;
		NEX = 1;
		SWAP = 1;
	}
	else
	{
		CUR = 1;
		NEX = 0;
		SWAP = 0;
	}

	for (int i = 0; i < ACT_NUM_GRID_X; i++)
	{
		for (int j = 0; j < ACT_NUM_GRID_Y; j++)
		{

			// check life
			bool yn = this->check_life(i, j, CUR);

			// set next generation
			if (yn && L_OVERHEAT[i][j] == false)
			{
				L_HOWLONG[i][j] += 0.005;
				if (L_HOWLONG[i][j] > 1.0)
				{
					L_HOWLONG[i][j] = 1.0;
					L_OVERHEAT[i][j] = true;
				}
			}

			if (L_OVERHEAT[i][j] == true)
			{
				L_HOWLONG[i][j] -= 0.005;

				if (L_HOWLONG[i][j] < 0.0)
				{
					L_HOWLONG[i][j] = 0.0;
					L_OVERHEAT[i][j] = false;
				}
			}



			if (L_OVERHEAT[i][j] == false)
			{
				L_ALIVE[NEX][i][j] = yn;
			}
			else
			{
				L_ALIVE[NEX][i][j] = false;
			}






			// generate animation from current cell
			int JUDGE = rand() % cell_kakuritu;

			if ( L_ALIVE[CUR][i][j] && JUDGE == 0 )
			{
				// decider animation index
				int ANIM_IDX = rand() % 26;
				// decide size
				float neighbor_coef = (float)L_HOWMANY_NEIGHBOR[i][j] * 0.5 + 1.0;


				float RANGED_SIZE = (float)(rand() % 1000) * 0.001;
				RANGED_SIZE = pow(RANGED_SIZE, 8.0);
				RANGED_SIZE = fmax(0.1, RANGED_SIZE);
				RANGED_SIZE = sizeRange[0] + (RANGED_SIZE * (sizeRange[1] - sizeRange[0]))*neighbor_coef;
				
				// limit ranged size
				RANGED_SIZE = fminf(RANGED_SIZE, 10.0);



				// decide rotation
				float tempRotation = (float)(rand() % 360);





				// decide speed of animation
				float speedCoef = 1.0;
				
				if (RANGED_SIZE < 0.1)
				{
					speedCoef = 1.0;
				}
				else
				{
					speedCoef = 1.0 / (RANGED_SIZE*20.0);
					speedCoef = fmax(0.07, speedCoef);
				}

				float tempSpeed = cell_anim_speed * speedCoef;


				//******* temp
				if (ANIM_IDX != 0 && ANIM_IDX != 6 )
				{
					tempSpeed *= 0.2;
				}
				//***** temp




				if (is_kill_all == false)
				{
					mc_obj->generateFromGM(ANIM_IDX,
						L_XY_POS[i][j][0],
						L_XY_POS[i][j][1],
						RANGED_SIZE,
						tempSpeed,
						tempRotation
					);
				}
			}


			// Count up active cell
			if (L_ALIVE[CUR][i][j])
			{
				num_active_cell++;
			}

		} // for j
	} // for i



	// if cell are less than minimum amount, generate cell 

	if (num_active_cell < minimum_active_cell)
	{
		//printf("active cell %d\n", num_active_cell);
		// add new cell
		for (int i = 0; i < ACT_NUM_GRID_X; i++)
		{
			for (int j = 0; j < ACT_NUM_GRID_Y; j++)
			{
				int temp = rand() % 100;

				if (temp == 0)
				{
					L_ALIVE[NEX][i][j] = true;
				}
			}
		}
	}
}




bool lifeGameManager::check_life(int x, int y, int cur)
{
	// check neighborhood
	int pX[3];
	int pY[3];

	if (x == 0)
	{
		pX[0] = ACT_NUM_GRID_X - 1;
		pX[1] = x;
		pX[2] = x + 1;
	}
	else if (x == ACT_NUM_GRID_X - 1)
	{
		pX[0] = x - 1;
		pX[1] = x;
		pX[2] = 0;
	}
	else
	{
		pX[0] = x - 1;
		pX[1] = x;
		pX[2] = x + 1;
	}


	if (y == 0)
	{
		pY[0] = ACT_NUM_GRID_Y - 1;
		pY[1] = y;
		pY[2] = y + 1;
	}
	else if (y == ACT_NUM_GRID_Y - 1)
	{
		pY[0] = y - 1;
		pY[1] = y;
		pY[2] = 0;
	}
	else
	{
		pY[0] = y - 1;
		pY[1] = y;
		pY[2] = y + 1;
	}


	int IDX[8][2];
	IDX[0][0] = pX[0];	IDX[0][1] = pY[0];
	IDX[1][0] = pX[1];	IDX[1][1] = pY[0];
	IDX[2][0] = pX[2];	IDX[2][1] = pY[0];

	IDX[3][0] = pX[0];	IDX[3][1] = pY[1];
	IDX[4][0] = pX[2];	IDX[4][1] = pY[1];

	IDX[5][0] = pX[0];	IDX[5][1] = pY[2];
	IDX[6][0] = pX[1];	IDX[6][1] = pY[2];
	IDX[7][0] = pX[2];	IDX[7][1] = pY[2];



	// count up how many neighbor are alive.
	int COUNT = 0;
	bool isALIVE = L_ALIVE[cur][x][y];
	bool nextALIVE = false;

	for (int i = 0; i < 8; i++)
	{
		bool yn = L_ALIVE[cur][IDX[i][0]][IDX[i][1]];

		if (yn) { COUNT++; }
	}

	L_HOWMANY_NEIGHBOR[x][y] = COUNT;



	if (isALIVE)
	{
		if (COUNT == 2 || COUNT == 3)
		{
			// still alive
			nextALIVE = true;
		}
		else
		{
			// die
			nextALIVE = false;
		}
	}
	else
	{
		if (COUNT == 3)
		{
			// alive
			nextALIVE = true;
		}
		else
		{
			// still die
			nextALIVE = false;
		}
	}



	return nextALIVE;
}



void lifeGameManager::clear_all_cell()
{
	for (int i = 0; i < ACT_NUM_GRID_X; i++)
	{
		for (int j = 0; j < ACT_NUM_GRID_Y; j++)
		{
			L_ALIVE[0][i][j] = L_ALIVE[1][i][j] = false;
			L_HOWLONG[i][j] = 0.0;
			L_OVERHEAT[i][j] = false;
		}
	}
}
