#pragma once

#define CIRCLE_DIVISION 182 // center and final +2

class ovalManager
{
public:
	ovalManager();
	~ovalManager();

	int num_vertex;

	// v, c, aux
	float OVAL_V[CIRCLE_DIVISION][4];
	float OVAL_C[CIRCLE_DIVISION][4];
	float OVAL_AUX[CIRCLE_DIVISION][4];

	// variables
	float OVAL_CENTER[2];
	float OVAL_CENTER_VELOCITY[2];
	float OVAL_VELOCITY[CIRCLE_DIVISION][2];
	float OVAL_SIZE;
	float EDGE_LENGTH;

	float WAVE_CYCLE[8];
	float WAVE_CYCLE_add[8];


	void update_oval();

	void set_new_oval( float size );
	float* get_oval_v_pointer();
	float* get_oval_c_pointer();
	float* get_oval_aux_pointer();
};

