#pragma once

#define BUG_DIVISION 20


class flyingBugManager
{
public:
	flyingBugManager();
	~flyingBugManager();

	int numVertex;
	float bug_v[BUG_DIVISION][6][4];
	float bug_t[BUG_DIVISION][6][4];
	float bug_count[BUG_DIVISION];
	float unit_X[2];
	float unit_Y[2];

	void bug_loop();
	void start_bug(float start_x, float start_y, float end_x, float end_y);

	float* get_bug_v_pointer();
	float* get_bug_t_pointer();
};

