#pragma once

// utility class
#include "OpenGLInitializerClass.h"
#include "WindowManagerClass.h"
#include "NotificationCenterClass.h"
#include "png24LoaderClass.h"
#include "shaderLoaderClass.h"
#include "stopWatch.h"
#include "matrixClass.h"

#include "OSC.h"
#include "UDPClass.h"


#include "animationManager.h"
//#include "generateManager.h" // unused
#include "lifeGameManager.h"
#include "ovalManager.h"
#include "windParticleManager.h"
#include "flyingBugManager.h"


#include "soundClass.h"


#define NUM_TEXTURE_IMG 27
#define NUM_OVAL 8

// last 3 texture is used for FBO (29, 30, 31)
#define RENDER_width 9600     // 9600 / 2
#define RENDER_height 1200    // 1200 / 2
#define WINDOW_width 4800
#define WINDOW_height 600

#define DIVISION_X 21
#define DIVISION_Y 11
#define NUM_IDX_ARRAY 440 // (21 * 2 + 2 ) * ( 11 - 1 )

class mainController
{
public:

	OpenGLInitializerClass* OPENGL_INITIALIZER_obj;
	WindowManagerClass* WINDOW_MANAGER_obj;
	NotificationCenterClass* NOTIFICATION_CENTER_obj;
	shaderLoaderClass* SHADER_LOADER_obj;
	png24LoaderClass* PNG24_LOADER_obj;
	stopWatch* STOP_WATCH_obj;

	UDPClass* udp_obj;
	OSC* OSC_wind_obj;
	OSC* OSC_hokuyo_obj;

	animationManager* animationManager_obj[NUM_TEXTURE_IMG];
	//generateManager* generateManager_obj;
	lifeGameManager* lifegameManager_obj;
	lifeGameManager* lifegameManager_BIG_obj;
	ovalManager* ovalManager_obj[NUM_OVAL];
	windParticleManager* windParticleManager_obj;
	flyingBugManager* flyingBugManager_obj;

	soundClass* sound_obj;


	// for drawing
	HWND hWnd_OpenGL;
	int NUM_LOAD_ANIMATION;

	// drawing class
	matrixClass* mvp_matrix_obj;

	int mute_safety_counter;
	bool isKill;


	// var for OpenGL
	//:::::: vertex for debug grid point
	GLfloat pointVer[200][50][4];
	GLfloat pointCol[200][50][4];
	GLfloat pointAux[200][50][4];

	//::::::: vertex of final board :::::::::::
	GLfloat f_vert[5][DIVISION_Y][DIVISION_X][4];
	GLfloat f_texc[5][DIVISION_Y][DIVISION_X][4];
	GLint idx_vert[NUM_IDX_ARRAY]; // (21*2 + 2) * (11-1) // index array for triangle strip


	// ::::: var for name
	GLuint FBO_toTEX_Name;
	GLuint TEX_forFBO_Name[3]; // col1, col2, depth

	GLuint Sampler_Name;
	GLuint TEX_Name[NUM_TEXTURE_IMG];


	GLuint VAO_TEST_Name;
	GLuint VBO_TEST_Name[3]; // vert color texcoord
	GLfloat blackV[4][4];
	GLfloat act_black_alpha = 0.9;

	//::::::::::::::::::::::::::::::::::::
	GLuint VAO_ANIMATION_Name;
	GLuint VBO_ANIMATION_Name[NUM_TEXTURE_IMG][2]; // vert, texcoord
	//::::::::::::::::::::::::::::::::::::
	GLuint VAO_POINT_Name;
	GLuint VBO_POINT_Name[3]; // vert, color, somedata
	//::::::::::::::::::::::::::::::::::::
	GLuint VAO_FINAL_BOARD_Name;
	GLuint VBO_FINAL_BOARD_Name[2]; // vert texcoord
	GLuint EBO_FINAL_BOARD_Name; // element array buffer

	GLuint TEST_VS;
	GLuint TEST_FS;
	GLuint TEST_PRG;
	GLint UNF_TEST_MVP_MATRIX;
	//::::::::::::::::::::::::::::::::::::
	GLuint ANIMATION_VS;
	GLuint ANIMATION_FS;
	GLuint ANIMATION_PRG;
	GLint UNF_ANIMATION_MVP_MATRIX;
	GLint UNF_ANIMATION_TEX_INDEX;
	//::::::::::::::::::::::::::::::::::::
	GLuint POINT_VS;
	GLuint POINT_FS;
	GLuint POINT_PRG;
	GLint UNF_POINT_MVP_MATRIX;
	//::::::::::::::::::::::::::::::::::::
	GLuint FINAL_VS;
	GLuint FINAL_FS;
	GLuint FINAL_PRG;
	GLint UNF_FINAL_MVP_MATRIX;
	GLint UNF_FINAL_TEX_INDEX;
	GLint UNF_FINAL_DEBUG_FLAG;


	// constructor
	mainController();
	~mainController();


	// function
	void setUp_application(HWND mainWindow);


	// 
	void calculate_finalboard_vert( int screen, 
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
		float texKaburi);


	// setup texture
	void setUp_TEXTURE();
	// setup VAO ( vertex array object )
	void setUp_VAO();
	// shader
	void setUp_SHADER();
	// FBO
	void setUp_FBO();


	// drawing
	void draw_to_FBO();
	void draw_to_FinalBoard();


	// from generateManager
	void generateFromGM(int N_TEX, float pX, float pY, float size, float speed, float angle);
};

