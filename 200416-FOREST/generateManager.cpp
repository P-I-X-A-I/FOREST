#include "stdafx.h"
#include "generateManager.h"
#include "mainController.h"

generateManager::generateManager()
{
	printf("init generationManager.\n");

	// init seed
	SEED_COUNTER = 0;

	for (int i = 0; i < MAX_SEED_NUM; i++)
	{
		this->reset_seed(i);
	}
}


generateManager::~generateManager()
{
}


void generateManager::reset_seed(int idx)
{
	SEED_ACTIVE[idx] = false;
	SEED_NUM_CHILD[idx] = 0;

	for (int c = 0; c < MAX_CHILD_NUM; c++)
	{
		SEED_CHILD_ACTIVE[idx][c] = false;

		SEED_CHILD_POS[idx][c][0] = -100.0;
		SEED_CHILD_POS[idx][c][1] = -100.0;

		SEED_CHILD_ANGLE[idx][c] = 0.0;
	}
}


void generateManager::generate_seed(int idx)
{
	// activate
	SEED_ACTIVE[idx] = true;

	// reset child count
	SEED_NUM_CHILD[idx] = 0;
	SEED_CHILD_ACTIVE[idx][0] = true;

	// set initial position
	SEED_CHILD_POS[idx][0][0] = (rand() % 200 - 100) * 0.05; // -5.0 ~ 5.0
	SEED_CHILD_POS[idx][0][1] = (rand() % 200 - 100) * 0.005; // -0.5 ~ 0.5

	// set initial angle
	SEED_CHILD_ANGLE[idx][0] = (rand() % 360)*1.0;
}



void generateManager::loop()
{

	int rSeed = rand() % 10;
	if (rSeed == 0)
	{
		// generate seed
		this->generate_seed(SEED_COUNTER);
		//printf("Gen seed %d\n", SEED_COUNTER);

		SEED_COUNTER++;

		if (SEED_COUNTER >= MAX_SEED_NUM)
		{
			SEED_COUNTER = 0;
		}
	}



	// /////////
	for (int i = 0; i < MAX_SEED_NUM; i++)
	{
		if (SEED_ACTIVE[i] == true)
		{

			// generate board
			for (int j = 0; j < MAX_CHILD_NUM; j++)
			{
				if (SEED_CHILD_ACTIVE[i][j])
				{
					// generate board
					mc_obj->generateFromGM(rand() % 26,
											SEED_CHILD_POS[i][j][0] + (rand()%100-50)*0.001,
											SEED_CHILD_POS[i][j][1] + (rand() % 100 - 50)*0.001,
											1.0,
											1.0,
											SEED_CHILD_ANGLE[i][j]);
				}
			}


			// judge if seed generate next child or not *********************
			for (int j = MAX_CHILD_NUM-1; j >= 0; j--)
			{
				if (SEED_CHILD_ACTIVE[i][j])
				{
					int gen_or_not = rand() % 6;
					if (gen_or_not < 2)
					{
						//printf("seed %d, child %d\n", i, j);
						// activate next child
						SEED_CHILD_ACTIVE[i][j + 1] = true;
						// deactivate self
						SEED_CHILD_ACTIVE[i][j] = false;

						// set next child position
						float angle = SEED_CHILD_ANGLE[i][j + 1] = SEED_CHILD_ANGLE[i][j] + (float)(rand() % 20 - 5);
						float dX = cos(angle * 0.01745329) * 0.02;
						float dY = sin(angle * 0.01745329) * 0.02;

						SEED_CHILD_POS[i][j + 1][0] = SEED_CHILD_POS[i][j][0] + dX;
						SEED_CHILD_POS[i][j + 1][1] = SEED_CHILD_POS[i][j][1] + dX;

						// count up num child
						SEED_NUM_CHILD[i]++;

						if (SEED_NUM_CHILD[i] >= MAX_CHILD_NUM - 1)
						{
							this->reset_seed(i);
						}
					}
				}
			}

		


		} // if SEED[i] is true

	} // for i < MAX_SEED_NUM


	/*
	mc_obj->generateFromGM( 22,
							(rand()%200-100)*0.05,
							(rand()%200-100)*0.01);
							*/
}
