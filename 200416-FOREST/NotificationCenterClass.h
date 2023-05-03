#pragma once
class mainController;
class OSC;

class NotificationCenterClass
{
public:
	NotificationCenterClass();
	~NotificationCenterClass();

	mainController* mainController_obj;
	OSC* hokuyo_osc_obj;
	OSC* wind_osc_obj;

	BOOL isBigOn;
	BOOL dirtyFlag;
	BOOL isBugStart;
	float bugValue[4];

	float wind_speed;
	int wind_degree;

	// from WindowManager
	void messageFromGUI(UINT message, int GUI_ID, int EVENT_CODE, LPARAM lParam);
	void keyEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void mouseEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// from OSC
	void receiveOSC(char* address);
};

