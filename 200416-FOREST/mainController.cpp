#include "stdafx.h"
#include "mainController.h"


mainController::mainController()
{
	printf("mainControlelr init\n");

	srand(time(NULL));

	// init utility class
	OPENGL_INITIALIZER_obj = new OpenGLInitializerClass();
	WINDOW_MANAGER_obj = new WindowManagerClass();
	NOTIFICATION_CENTER_obj = new NotificationCenterClass();
	SHADER_LOADER_obj = new shaderLoaderClass();
	PNG24_LOADER_obj = new png24LoaderClass();
	STOP_WATCH_obj = new stopWatch();

	WINDOW_MANAGER_obj->set_notification_center(NOTIFICATION_CENTER_obj);

	// for geometry
	mvp_matrix_obj = new matrixClass();

	NUM_LOAD_ANIMATION = 0;

	mute_safety_counter = 0;
	isKill = false;

	blackV[0][0] = -1.0;
	blackV[0][1] = 1.0;
	blackV[0][2] = 0.0;
	blackV[0][3] = 1.0;

	blackV[1][0] = -1.0;
	blackV[1][1] = -1.0;
	blackV[1][2] = 0.0;
	blackV[1][3] = 1.0;

	blackV[2][0] = 1.0;
	blackV[2][1] = 1.0;
	blackV[2][2] = 0.0;
	blackV[2][3] = 1.0;

	blackV[3][0] = 1.0;
	blackV[3][1] = -1.0;
	blackV[3][2] = 0.0;
	blackV[3][3] = 1.0;
}


mainController::~mainController()
{
}



void mainController::setUp_application(HWND mainWindow)
{
	printf("\n ***mainController::setUp_application *** \n");

	// init communication ***************************
	bool yn1, yn2;

	printf("\n*** INIT COMMUNICATION CLASS ***\n");
	udp_obj = new UDPClass();
	yn1 = udp_obj->createReceiveSocket(3333);
	yn2 = udp_obj->createSendSocket("10.10.10.10", 2222);

	printf("Create UDP Receive socket %d\n", yn1);
	printf("Create UDP Send socked %d\n", yn2);


	OSC_hokuyo_obj = new OSC();
	OSC_wind_obj = new OSC();

	NOTIFICATION_CENTER_obj->hokuyo_osc_obj = OSC_hokuyo_obj;
	NOTIFICATION_CENTER_obj->wind_osc_obj = OSC_wind_obj;

	/////////////////////////////////////
	OSC_hokuyo_obj->notification_obj = NOTIFICATION_CENTER_obj;
	yn1 = OSC_hokuyo_obj->set_ReceiveSocket(10001);
	yn2 = OSC_hokuyo_obj->set_SendSocket("10.0.0.4", 2121);
	OSC_hokuyo_obj->setReceiveAddress("/blob");
	OSC_hokuyo_obj->setReceiveAddress("/forest/mute");
	OSC_hokuyo_obj->setReceiveAddress("/forest/big");
	OSC_hokuyo_obj->setReceiveAddress("/forest/bug");

	printf("Create OSC_hokuyo Receive socket %d\n", yn1);
	printf("Create OSC_hokuyo Send socket %d\n", yn2);
	///////////////////////////////////////
	OSC_wind_obj->notification_obj = NOTIFICATION_CENTER_obj;
	yn1 = OSC_wind_obj->set_ReceiveSocket(9998);
	OSC_wind_obj->setReceiveAddress("/wind/direction");
	OSC_wind_obj->setReceiveAddress("/wind/speed");

	printf("Create OSC_wind Receive Socket %d\n", yn1);
////////////////////////////////////////////////


	//create dummy window
	printf("\n *** INIT OPENGL ***\n");
	HWND dummyWnd = WINDOW_MANAGER_obj->create_borderless_window(L"dummy");
	WINDOW_MANAGER_obj->set_window_position(dummyWnd, 0, 0);
	WINDOW_MANAGER_obj->set_window_size(dummyWnd, 100, 100);
	WINDOW_MANAGER_obj->set_window_level(dummyWnd, 1); // normal

	// create main window
	hWnd_OpenGL = WINDOW_MANAGER_obj->create_borderless_window(L"mainWindow");
	WINDOW_MANAGER_obj->set_window_position(hWnd_OpenGL, 400, 0);
	WINDOW_MANAGER_obj->set_window_size(hWnd_OpenGL, 192, 120);
	WINDOW_MANAGER_obj->set_window_level(hWnd_OpenGL, 2); // topmost

	// init OpenGL
	OPENGL_INITIALIZER_obj->init_OpenGL_functions(dummyWnd, hWnd_OpenGL);

	// close dummy window
	WINDOW_MANAGER_obj->close_window(dummyWnd);

	// setup OpenGL basic status in OpenGL Initializer class
	OPENGL_INITIALIZER_obj->set_OpenGL_status();



	//////////////////// Generate Manager ////////////////
	//printf("\n *** INIT GENERATION MANAGER ***\n");
	//generateManager_obj = new generateManager();
	//generateManager_obj->mc_obj = this;

	// lifegame manager
	lifegameManager_obj = new lifeGameManager();
	lifegameManager_obj->mc_obj = this;
	//lifegameManager_obj->reset_condition(640, 72);
	//lifegameManager_obj->set_size_range(0.01, 0.1);
	lifegameManager_obj->set_minimum_active_cell(150);
	//lifegameManager_obj->set_cell_kakuritu(6);
	lifegameManager_obj->set_cell_anim_speed(10.0);


	// lifegame manager 2 ( bigger ) // not used
	lifegameManager_BIG_obj = new lifeGameManager();
	lifegameManager_BIG_obj->mc_obj = this;
	lifegameManager_BIG_obj->reset_condition(240, 27);
	lifegameManager_BIG_obj->set_size_range(0.1, 0.4);
	lifegameManager_BIG_obj->set_minimum_active_cell(40);
	lifegameManager_BIG_obj->set_cell_kakuritu(100);
	lifegameManager_BIG_obj->set_cell_anim_speed(20.0);


	////////// ANIMATION manager //////////////////////////////////

	printf("\n *** INIT ANIMATION MANAGER *** \n");

	for (int i = 0; i < NUM_TEXTURE_IMG; i++)
	{
		animationManager_obj[i] = new animationManager();
	}



	//////////////////////////////////////////////////////////////////

	// animation 0 
	animationManager_obj[0]->set_animation_frame(20, 14, 280);
	animationManager_obj[0]->rect_ratio = 600.0 / 856.0;
	// animation 1
	animationManager_obj[1]->set_animation_frame(40, 1, 40);
	animationManager_obj[1]->rect_ratio = 204.8 / 223.0;
	// animation 2
	animationManager_obj[2]->set_animation_frame(40, 1, 40);
	animationManager_obj[2]->rect_ratio = 204.8 / 435.0;
	// animation 3
	animationManager_obj[3]->set_animation_frame(40, 1, 40);
	animationManager_obj[3]->rect_ratio = 204.8 /274.0;
	// animation 4
	animationManager_obj[4]->set_animation_frame(30, 1, 30);
	animationManager_obj[4]->rect_ratio = 110.0 / 190.0;
	// animation 5
	animationManager_obj[5]->set_animation_frame(30, 1, 30);
	animationManager_obj[5]->rect_ratio = 150.0 / 300.0;
	// animation 6
	animationManager_obj[6]->set_animation_frame(20, 11, 220);
	animationManager_obj[6]->rect_ratio = 400.0 / 725.5454;
	// animation 7
	animationManager_obj[7]->set_animation_frame(40, 1, 40);
	animationManager_obj[7]->rect_ratio = 204.8 / 512.0;
	// animation 8
	animationManager_obj[8]->set_animation_frame(40, 1, 40);
	animationManager_obj[8]->rect_ratio = 204.8 / 364.0;
	// animation 9
	animationManager_obj[9]->set_animation_frame(30, 1, 30);
	animationManager_obj[9]->rect_ratio = 240.0 / 410.0;
	// animation 10
	animationManager_obj[10]->set_animation_frame(40, 1, 40);
	animationManager_obj[10]->rect_ratio = 204.8 / 339.0;
	// animation 11
	animationManager_obj[11]->set_animation_frame(30, 1, 30);
	animationManager_obj[11]->rect_ratio = 200.0 / 430.0;
	// animation 12
	animationManager_obj[12]->set_animation_frame(40, 1, 40);
	animationManager_obj[12]->rect_ratio = 200.0 / 450.0;
	// animation 13
	animationManager_obj[13]->set_animation_frame(30, 1, 30);
	animationManager_obj[13]->rect_ratio = 273.0666 / 379.0;
	// animation 14
	animationManager_obj[14]->set_animation_frame(40, 1, 40);
	animationManager_obj[14]->rect_ratio = 204.8 / 384.0;
	// animation 15
	animationManager_obj[15]->set_animation_frame(40, 1, 40);
	animationManager_obj[15]->rect_ratio = 204.8 / 315.0;
	// animation 16
	animationManager_obj[16]->set_animation_frame(40, 1, 40);
	animationManager_obj[16]->rect_ratio = 204.8 / 314.0;
	// animation 17
	animationManager_obj[17]->set_animation_frame(40, 1, 40);
	animationManager_obj[17]->rect_ratio = 180.0 / 415.0;
	// animation 18
	animationManager_obj[18]->set_animation_frame(30, 1, 30);
	animationManager_obj[18]->rect_ratio = 220.0 / 420.0;
	// animation 19
	animationManager_obj[19]->set_animation_frame(30, 1, 30);
	animationManager_obj[19]->rect_ratio = 250.0 / 470.0;
	// animation 20
	animationManager_obj[20]->set_animation_frame(40, 1, 40);
	animationManager_obj[20]->rect_ratio = 204.8 / 335.0;
	// animation 21
	animationManager_obj[21]->set_animation_frame(40, 1, 40);
	animationManager_obj[21]->rect_ratio = 204.8 / 210.0;
	// animation 22
	animationManager_obj[22]->set_animation_frame(30, 1, 30);
	animationManager_obj[22]->rect_ratio = 273.0666 / 173.0;
	// animation 23
	animationManager_obj[23]->set_animation_frame(30, 1, 30);
	animationManager_obj[23]->rect_ratio = 273.06666 / 324.0;
	// animation 24
	animationManager_obj[24]->set_animation_frame(40, 1, 40);
	animationManager_obj[24]->rect_ratio = 200.0 / 420.0;
	// animation 25
	animationManager_obj[25]->set_animation_frame(40, 1, 40);
	animationManager_obj[25]->rect_ratio = 204.8 / 439.0;

	// animation 26



	NUM_LOAD_ANIMATION = 26;
	

	//////////////////////////////////////////////////////////////////

	// oval manager
	//printf("\n*** OVAL MANAGER ***\n");
	//for (int i = 0; i < NUM_OVAL; i++)
	//{
	//	ovalManager_obj[i] = new ovalManager();
	//	ovalManager_obj[i]->set_new_oval( 0.1*i + 0.1 );
	//}

	//////////////////////////////////////////////////////////////////

	// wind particle manager
	windParticleManager_obj = new windParticleManager();
	windParticleManager_obj->screen_ratio = (float)WINDOW_width / (float)WINDOW_height;

	//////////////////////////////////////////////////////////////////

	//printf("\n *** SOUND CLASS ***\n");

	//sound_obj = new soundClass();

	
	//////////////////////////////////////////////////////////////////
	flyingBugManager_obj = new flyingBugManager();

	//////////////////////////////////////////////////////////////////

	this->setUp_TEXTURE();
	this->setUp_VAO();
	this->setUp_SHADER();
	this->setUp_FBO();

	// set mainwindow size and position
	WINDOW_MANAGER_obj->set_window_position(hWnd_OpenGL, 0, 0);
	WINDOW_MANAGER_obj->set_window_level(hWnd_OpenGL, 2);
	WINDOW_MANAGER_obj->set_window_size(hWnd_OpenGL, WINDOW_width, WINDOW_height);



	// generate new final board vertex ( for mapping ) topLeft, topRight, btmLeft,  btmRight, topangle, btmangle, topshift, btmshift, texKaburi

	this->calculate_finalboard_vert(0,
								-0.955, 0.952, 
								-0.87, 0.87, 
								(90.0 + 17.5), (90.0 - 18.2),
								(90.0 + 39.5), (90.0 - 39.5),
								-0.05, -1.67,
								0.0105);

	this->calculate_finalboard_vert(1,
								-0.926, 0.928,
								-0.857, 0.857,
									(90.0 + 21.5), (90.0 - 20.5),
									(90.0 + 37.67), (90.0 - 39.5),
									-0.0275, -1.68,
									0.0105);

	this->calculate_finalboard_vert(2,
									-0.945, 0.95,
									-0.86, 0.86,
									(90.0 + 20.3), (90.0 - 20.5),
									(90.0 + 38.5), (90.0 - 39.6),
									-0.03, -1.66,
									0.0105);

	this->calculate_finalboard_vert(3,
									-0.926, 0.934,
									-0.855, 0.865,
									(90.0 + 23.3), (90.0 - 22.3),
									(90.0 + 38.2), (90.0 - 39.3),
									-0.005, -1.675,
									0.0105);

	this->calculate_finalboard_vert(4,
									-0.955, 0.955,
									-0.863, 0.865,
									(90.0 + 20.1), (90.0 - 20.0), 
									(90.0 + 38.3), (90.0 - 39.0),
									-0.025, -1.665,
									0.0105);

	// hide mouse cursor
	ShowCursor(FALSE);
}



void mainController::generateFromGM( int N_TEX, float pX, float pY, float size, float speed, float angle)
{
	//float size = rand() % 100 * 0.0005 + 0.01; // 0.01 ~ 0.06

	animationManager_obj[N_TEX]->start_board(pX, pY, angle, size, speed, 0.0);
}



void mainController::calculate_finalboard_vert(int screen, 
	float topLeft,
	float topRight,
	float bottomLeft,
	float bottomRight,
	float topAngleStart,
	float topAngleEnd,
	float bottomAngleStart,
	float bottomAngleEnd,
	float topEdgeShift,
	float bottomEdgeShift,
	float texKaburi)
{
	float RATIO = (float)WINDOW_width / (float)WINDOW_height;
	float screenSize = RATIO / 5.0;
	float screenCenter = (screenSize * -4.0) + screen * 2.0 * screenSize;
	float scWidth_top = topRight - topLeft;
	float scWidth_btm = bottomRight - bottomLeft;

	// length scale of top & bottom edge
	float act_scSize_top = screenSize * scWidth_top;
	float act_scSize_btm = screenSize * scWidth_btm;
	float incX_top = act_scSize_top / (float)(DIVISION_X - 1);
	float incX_btm = act_scSize_btm / (float)(DIVISION_X - 1);

	// curve angle range of top & bottom edge
	float topAngle[2];
	float bottomAngle[2];
	topAngle[0] = topAngleStart; // max angle
	topAngle[1] = topAngleEnd; // min angle
	bottomAngle[0] = bottomAngleStart; // max angle
	bottomAngle[1] = bottomAngleEnd; // min angle

	float start_topAngle = topAngle[0];
	float start_bottomAngle = bottomAngle[0];
	float inc_topAngle = (topAngle[0] - topAngle[1]) / (float)(DIVISION_X - 1);
	float inc_bottomAngle = (bottomAngle[0] - bottomAngle[1]) / (float)(DIVISION_X - 1);


	float vecToBottom[DIVISION_X][2];

	for (int x = 0; x < DIVISION_X; x++)
	{
		float top_radian = (start_topAngle - inc_topAngle * x)*0.0174532925;
		float bottom_radian = (start_bottomAngle - inc_bottomAngle * x)*0.0174532925;

		f_vert[screen][0][x][0] = (screenCenter + topLeft*screenSize) + (x * incX_top);
		f_vert[screen][0][x][1] = 1.0 * sin(top_radian) + topEdgeShift;

		f_vert[screen][DIVISION_Y - 1][x][0] = (screenCenter + bottomLeft*screenSize) + (x * incX_btm);
		f_vert[screen][DIVISION_Y - 1][x][1] = 1.0 * sin(bottom_radian) + bottomEdgeShift;


		// vec to bottom edge
		vecToBottom[x][0] = f_vert[screen][DIVISION_Y - 1][x][0] - f_vert[screen][0][x][0];
		vecToBottom[x][1] = f_vert[screen][DIVISION_Y - 1][x][1] - f_vert[screen][0][x][1];

		vecToBottom[x][0] /= (float)(DIVISION_Y - 1);
		vecToBottom[x][1] /= (float)(DIVISION_Y - 1);
	}


	// interpolation
	

	for (int y = 1; y < DIVISION_Y - 1; y++)
	{
		for (int x = 0; x < DIVISION_X; x++)
		{
			f_vert[screen][y][x][0] = f_vert[screen][0][x][0] + vecToBottom[x][0] * y;
			f_vert[screen][y][x][1] = f_vert[screen][0][x][1] + vecToBottom[x][1] * y;
		}
	}



	// tex coord
	float tRange = 0.95;
	float minRange = 0.025;
	float maxRange = 0.975;
	
	float eachRange = tRange / 5.0;
	float eachLeftEdge = minRange + (eachRange*screen) - texKaburi;
	
	float finalRange = eachRange + texKaburi * 2.0;
	float t_incX = finalRange / (float)(DIVISION_X - 1);
	float t_incY = 1.0 / (float)(DIVISION_Y - 1);


	for (int y = 0; y < DIVISION_Y; y++)
	{
		for (int x = 0; x < DIVISION_X; x++)
		{
			float alpha = 1.0;
			if (screen == 0)
			{
				if (x == DIVISION_X - 2)
				{
					alpha = 1.0;
				}
				else if (x == DIVISION_X - 1)
				{
					alpha = 0.5;
				}
			}
			else if (screen == 4)
			{
				if (x == 0)
				{
					alpha = 0.5;
				}
				else if (x == 1)
				{
					alpha = 1.0;
				}
			}
			else
			{
				if (x == 1 || x == DIVISION_X - 2)
				{
					alpha = 1.0;
				}
				else if (x == 0 || x == DIVISION_X - 1)
				{
					alpha = 0.5;
				}
			}

			f_texc[screen][y][x][0] = x * t_incX + eachLeftEdge;
			f_texc[screen][y][x][1] = y * t_incY;
			f_texc[screen][y][x][2] = alpha;
			f_texc[screen][y][x][3] = 0.0;
		
		}
	}
}
