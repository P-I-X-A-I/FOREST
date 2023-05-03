#include "stdafx.h"
#include "NotificationCenterClass.h"
#include "mainController.h"
#include "OSC.h"

NotificationCenterClass::NotificationCenterClass()
{
	printf("NotificationCenterClass init.\n");

	isBigOn = false;
	dirtyFlag = false;
	isBugStart = false;
	bugValue[0] = 0.0;
	bugValue[1] = 0.0;
	bugValue[2] = 0.0;
	bugValue[3] = 0.0;

	wind_speed = 0.0;
	wind_degree = 0;
}


NotificationCenterClass::~NotificationCenterClass()
{
}


void NotificationCenterClass::messageFromGUI(UINT message, int GUI_ID, int EVENT_CODE, LPARAM lParam)
{
	// message from GUI

	int HI = HIWORD(lParam);
	int LO = HIWORD(lParam);

	NMHDR* nmHandler = (NMHDR*)lParam; // for WM_NOTIFY message

	switch (GUI_ID)
	{
	case 1000:
		// if it "button"
		if (EVENT_CODE == BN_CLICKED) // when clicked
		{
			printf("button clicked\n");
		}
		break;
	case 2000:
		// if it "slider"
		if (nmHandler->code == 0xfffffff0)
		{
			printf("released %d\n", EVENT_CODE);
		}
		else if (nmHandler->code == 0xfffffff4)
		{
			// should not use this code, 
			printf("moved %d\n", EVENT_CODE);
		}
		break;
	case 7000:
		// if it "combobox"
		if (EVENT_CODE == CBN_DROPDOWN) // when open
		{
			printf("dropdown\n");
		}
		else if (EVENT_CODE == CBN_CLOSEUP) // when close 
		{
			printf("close up\n");
		}
		else if (EVENT_CODE == CBN_SELCHANGE) // when sel change
		{
			printf("select change\n");
		}
		else if (EVENT_CODE == CBN_SETFOCUS) // when get focused
		{
			printf("set focused\n");
		}
		else if (EVENT_CODE == CBN_KILLFOCUS) // when lost focuse
		{
			printf("lost focus\n");
		}
		break;
	case 8000:
		// if it listbox

		break;
	default:
		break;
	}
}


void NotificationCenterClass::keyEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// key event from WindowManager
	printf("window %d key event %d %d\n", hWnd, wParam, lParam);
	float tempVal = 0.025;
	if (lParam = 65537) // when key pushed
	{
		switch (wParam)
		{
		case VK_UP:
			break;
		case VK_RETURN:
			break;
		case VK_SPACE:
			break;
		case VK_ESCAPE: //esc
			exit(0);
			break;

		default:
			break;
		}
	}
}


void NotificationCenterClass::mouseEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// mouse event form WindowManager
	//printf("mouse event\n");
}



//***************************************

void NotificationCenterClass::receiveOSC(char* address)
{
	if (strcmp(address, "/blob") == 0)
	{
		// do something
	}
	else if (strcmp(address, "/forest/mute") == 0)
	{
		printf("mute\n");
		dirtyFlag = true;
	}
	else if (strcmp(address, "/forest/big") == 0)
	{
		printf("big\n");
		isBigOn = true;
	}
	else if (strcmp(address, "/forest/bug") == 0)
	{
		isBugStart = true;
		bugValue[0] = hokuyo_osc_obj->getArgAsFloat(0);
		bugValue[1] = hokuyo_osc_obj->getArgAsFloat(1);
		bugValue[2] = hokuyo_osc_obj->getArgAsFloat(2);
		bugValue[3] = hokuyo_osc_obj->getArgAsFloat(3);

		printf("bug %f %f %f %f\n", bugValue[0], bugValue[1], bugValue[2], bugValue[3]);



	}
	else if (strcmp(address, "/wind/speed")==0)
	{
		// 0.0 - 30.0
		wind_speed = wind_osc_obj->getArgAsFloat(0);
	}
	else if (strcmp(address, "/wind/direction")==0)
	{
		// 0 - 359
		wind_degree = wind_osc_obj->getArgAsInt(0);
	}
	//printf("Notification : %s\n", address);
}
