#include "stdafx.h"
#include "animationManager.h"


animationManager::animationManager()
{

	printf("animationManager init\n");

	// init variable
	num_animation_frame = 1;
	num_board = MAX_NUM_BOARD;
	rect_ratio = 1.0;

	COUNTER = 0;

	for (int i = 0; i < MAX_NUM_FRAME; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			anim_texcoord_ref[i][j][0] = 0.0;
			anim_texcoord_ref[i][j][1] = 0.0;
		}
	}


	for (int i = 0; i < MAX_NUM_BOARD; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			VERT[i][j][0] = 0.0;
			VERT[i][j][1] = 0.0;
			VERT[i][j][2] = 0.0;
			VERT[i][j][3] = 1.0;

			act_VERT[i][j][0] = 0.0;
			act_VERT[i][j][1] = 0.0;
			act_VERT[i][j][2] = 0.0;
			act_VERT[i][j][3] = 1.0;

			TEXC[i][j][0] = 0.0;
			TEXC[i][j][1] = 0.0;
			TEXC[i][j][2] = 0.0;
			TEXC[i][j][3] = 0.0;
		}

		LIFE[i] = 0.0;
		LIFE_REDUCE[i] = 1.0;

		POSITION[i][0] = 0.0;
		POSITION[i][1] = 10.0;

		ANGLE[i] = 0.0;

		SIZE[i] = 0.1;

	} // for i < MAX_NUM_BOARD

}


animationManager::~animationManager()
{
}


void animationManager::set_animation_frame(int x, int y, int total)
{
	// set num animation frame
	num_animation_frame = total;

	// set texture coord ref;

	double UNIT_X = 1.0 / (double)x;
	double UNIT_Y = 1.0 / (double)y;


	for (int H = 0; H < y; H++)
	{
		for (int W = 0; W < x; W++)
		{
			int IDX = x * H + W;

			if (IDX >= total)
			{
				return;
			}

			double LEFT = W * UNIT_X;
			double RIGHT = LEFT + UNIT_X;
			double TOP = H * UNIT_Y;
			double BOTTOM = TOP + UNIT_Y;

			anim_texcoord_ref[IDX][0][0] = LEFT;	anim_texcoord_ref[IDX][0][1] = TOP;
			anim_texcoord_ref[IDX][1][0] = LEFT;	anim_texcoord_ref[IDX][1][1] = BOTTOM;
			anim_texcoord_ref[IDX][2][0] = RIGHT;	anim_texcoord_ref[IDX][2][1] = TOP;
			anim_texcoord_ref[IDX][3][0] = RIGHT;	anim_texcoord_ref[IDX][3][1] = BOTTOM;
		}
	}

	/*
	for (int i = 0; i < num_animation_frame; i++)
	{
		double LX = i * UNIT;
		double RX = LX + UNIT;

		anim_texcoord_ref[i][0][0] = LX;	anim_texcoord_ref[i][0][1] = 0.0;
		anim_texcoord_ref[i][1][0] = LX;	anim_texcoord_ref[i][1][1] = 1.0;
		anim_texcoord_ref[i][2][0] = RX;	anim_texcoord_ref[i][2][1] = 0.0;
		anim_texcoord_ref[i][3][0] = RX;	anim_texcoord_ref[i][3][1] = 1.0;
	}
	*/
}





void animationManager::update_animation()
{
	for (int i = 0; i < MAX_NUM_BOARD; i++)
	{
		if (LIFE[i] > 0.0)
		{
			// reduce life
			LIFE[i] -= LIFE_REDUCE[i];

			// set vertex
			for (int j = 0; j < 6; j++)
			{
				act_VERT[i][j][0] = VERT[i][j][0];
				act_VERT[i][j][1] = VERT[i][j][1];
			}



			// set texcoord
			int F_IDX = (num_animation_frame - 1) - (int)LIFE[i];

			// LT
			TEXC[i][0][0] = TEXC[i][1][0] = anim_texcoord_ref[F_IDX][0][0];
			TEXC[i][0][1] = TEXC[i][1][1] = anim_texcoord_ref[F_IDX][0][1];
			// LB
			TEXC[i][2][0] = anim_texcoord_ref[F_IDX][1][0];
			TEXC[i][2][1] = anim_texcoord_ref[F_IDX][1][1];
			// RT
			TEXC[i][3][0] = anim_texcoord_ref[F_IDX][2][0];
			TEXC[i][3][1] = anim_texcoord_ref[F_IDX][2][1];
			// RB
			TEXC[i][4][0] = TEXC[i][5][0] = anim_texcoord_ref[F_IDX][3][0];
			TEXC[i][4][1] = TEXC[i][5][1] = anim_texcoord_ref[F_IDX][3][1];


		}
		else
		{
			// reset board
			LIFE[i] = 0.0;

			for (int j = 0; j < 6; j++)
			{
				VERT[i][j][0] = act_VERT[i][j][0] = -100.0;
				VERT[i][j][1] = act_VERT[i][j][1] = -100.0;
			}

		}
	}// for i < MAX_NUM_BOARD
}



void animationManager::start_board(float pX, float pY, float angle, float size, float speed, float yugami)
{
	// set start position
	POSITION[COUNTER][0] = pX;
	POSITION[COUNTER][1] = pY;

	// set size
	SIZE[COUNTER] = size;

	// set life reduce

	LIFE_REDUCE[COUNTER] = speed;

	/////////////////////////////////////

	//unit vector
	ANGLE[COUNTER] = angle;

	float x[2], y[2];
	float radian = angle * 0.0174532925;

	x[0] = cos(radian);		x[1] = sin(radian);
	y[0] = -sin(radian);	y[1] = cos(radian);

	x[0] = x[0] * rect_ratio * SIZE[COUNTER];
	x[1] = x[1] * rect_ratio * SIZE[COUNTER];

	y[0] = y[0] * SIZE[COUNTER];
	y[1] = y[1] * SIZE[COUNTER];



	///////////////////////////////////////////////

	// set vertex ( texture animatio board )

	float ygm[4];
	for (int i = 0; i < 4; i++)
	{
		ygm[i] = 1.0 + (rand() % 100 - 50)*0.01*yugami;
	}


	VERT[COUNTER][0][0] = VERT[COUNTER][1][0] = pX + (-x[0] + y[0])*ygm[0];
	VERT[COUNTER][0][1] = VERT[COUNTER][1][1] = pY + (-x[1] + y[1])*ygm[0];

	VERT[COUNTER][2][0] = pX + (-x[0] - y[0])*ygm[1];
	VERT[COUNTER][2][1] = pY + (-x[1] - y[1])*ygm[1];

	VERT[COUNTER][3][0] = pX + (x[0] + y[0])*ygm[2];
	VERT[COUNTER][3][1] = pY + (x[1] + y[1])*ygm[2];

	VERT[COUNTER][4][0] = VERT[COUNTER][5][0] = pX + (x[0] - y[0])*ygm[3];
	VERT[COUNTER][4][1] = VERT[COUNTER][5][1] = pY + (x[1] - y[1])*ygm[3];


	// actual vertex
	for (int i = 0; i < 6; i++)
	{
		act_VERT[COUNTER][i][0] = pX;
		act_VERT[COUNTER][i][1] = pY;
	}


	//////////////////////////////////////////////////

	// set life & texcoord
	LIFE[COUNTER] = (float)num_animation_frame - 1.0;

	int T_IDX = (num_animation_frame - 1) - (int)LIFE[COUNTER];

	// LT
	TEXC[COUNTER][0][0] = TEXC[COUNTER][1][0] = anim_texcoord_ref[T_IDX][0][0];
	TEXC[COUNTER][0][1] = TEXC[COUNTER][1][1] = anim_texcoord_ref[T_IDX][0][1];
	// LB
	TEXC[COUNTER][2][0] = anim_texcoord_ref[T_IDX][1][0];
	TEXC[COUNTER][2][1] = anim_texcoord_ref[T_IDX][1][1];
	// RT
	TEXC[COUNTER][3][0] = anim_texcoord_ref[T_IDX][2][0];
	TEXC[COUNTER][3][1] = anim_texcoord_ref[T_IDX][2][1];
	// RB
	TEXC[COUNTER][4][0] = TEXC[COUNTER][5][0] = anim_texcoord_ref[T_IDX][3][0];
	TEXC[COUNTER][4][1] = TEXC[COUNTER][5][1] = anim_texcoord_ref[T_IDX][3][1];


	// set 0 or 1 to inverte white and black
	// set size to texcorrd[3] for blur shading
	float BorW = (float)(rand() % 100) * 0.01;
	if (SIZE[COUNTER] > 1.0)
	{
		BorW = 0.0;
	}

		for (int iter = 0; iter < 6; iter++)
		{
			TEXC[COUNTER][iter][2] = BorW;
			TEXC[COUNTER][iter][3] = size;
		}



	COUNTER++;
	if (COUNTER >= MAX_NUM_BOARD)
	{
		COUNTER = 0;
	}
}







float* animationManager::get_VERT_pointer()
{
	float* vPtr = &act_VERT[0][0][0];
	return vPtr;
}


float* animationManager::get_TEXC_pointer()
{
	float* tPtr = &TEXC[0][0][0];
	return tPtr;
}
