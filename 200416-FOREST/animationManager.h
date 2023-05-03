#pragma once

#define MAX_NUM_FRAME 600
#define MAX_NUM_BOARD 10000

class animationManager
{
public:
	animationManager();
	~animationManager();

	int COUNTER;

	int num_animation_frame;
	int num_board;
	double rect_ratio;
	float anim_texcoord_ref[MAX_NUM_FRAME][4][2];
	
	// vertex data
	float VERT[MAX_NUM_BOARD][6][4];
	float act_VERT[MAX_NUM_BOARD][6][4];
	float TEXC[MAX_NUM_BOARD][6][4]; // 3rd, 4th element are used for aux data.

	float LIFE[MAX_NUM_BOARD];
	float LIFE_REDUCE[MAX_NUM_BOARD];
	float POSITION[MAX_NUM_BOARD][2];
	float ANGLE[MAX_NUM_BOARD];
	float SIZE[MAX_NUM_BOARD];


	// function ************************************
	
	// setup
	void set_animation_frame(int x, int y, int total );
	// update
	void update_animation();


	// pointer to data
	float* get_VERT_pointer();
	float* get_TEXC_pointer();

	// emerge board
	void start_board(float pX, float pY, float angle, float size, float speed, float yugami);

};

