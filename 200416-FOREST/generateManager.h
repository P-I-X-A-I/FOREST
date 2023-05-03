#pragma once

#define MAX_SEED_NUM 256
#define MAX_CHILD_NUM 64

class mainController;

class generateManager
{
public:
	generateManager();
	~generateManager();


	mainController* mc_obj;

	int SEED_COUNTER;
	bool SEED_ACTIVE[MAX_SEED_NUM];
	int SEED_NUM_CHILD[MAX_SEED_NUM];

	bool SEED_CHILD_ACTIVE[MAX_SEED_NUM][MAX_CHILD_NUM];
	float SEED_CHILD_POS[MAX_SEED_NUM][MAX_CHILD_NUM][2];
	float SEED_CHILD_ANGLE[MAX_SEED_NUM][MAX_CHILD_NUM];


	void reset_seed(int idx);
	void generate_seed(int idx);

	// generation loop
	void loop();

};

