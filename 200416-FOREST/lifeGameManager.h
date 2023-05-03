
#pragma once

#define NUM_GRID_X 640 // 80 ( 16*5)
#define NUM_GRID_Y 72 // 9

class mainController;

class lifeGameManager
{
public:
	lifeGameManager();
	~lifeGameManager();


	mainController* mc_obj;

	BOOL is_kill_all;

	int ACT_NUM_GRID_X;
	int ACT_NUM_GRID_Y;
	int SWAP;
	bool L_ALIVE[2][NUM_GRID_X][NUM_GRID_Y]; // [2] for swap
	float L_HOWLONG[NUM_GRID_X][NUM_GRID_Y];
	int L_HOWMANY_NEIGHBOR[NUM_GRID_X][NUM_GRID_Y];
	bool L_OVERHEAT[NUM_GRID_X][NUM_GRID_Y];
	float L_XY_POS[NUM_GRID_X][NUM_GRID_Y][2];
	float sizeRange[2];
	int minimum_active_cell;
	int cell_kakuritu;
	float cell_anim_speed;

	// method
	void reset_condition(int W, int H);
	void set_size_range(float small, float big);
	void set_minimum_active_cell(int num);
	void set_cell_kakuritu(int val);
	void set_cell_anim_speed(float spe);

	void clear_all_cell();

	// generation loop
	void loop();
	bool check_life(int x, int y, int cur);

};

