#include "stdafx.h"
#include "mainController.h"


void mainController::setUp_VAO()
{
	printf("\n *** SETUP VAO *** \n");

	// test draw
	glGenVertexArrays(1, &VAO_TEST_Name);
	glGenBuffers(3, VBO_TEST_Name);

	// bind VAO
	glBindVertexArray(VAO_TEST_Name);

	// enable vertex array
	glEnableVertexAttribArray(0); // v
	//glEnableVertexAttribArray(1); // c
	//glEnableVertexAttribArray(2); // t

	//:::::::::::::::::::::::::::::::::::::::::

	// texture animation
	glGenVertexArrays(1, &VAO_ANIMATION_Name);
	glGenBuffers(2*NUM_TEXTURE_IMG, &VBO_ANIMATION_Name[0][0]);

	// bind VAO
	glBindVertexArray(VAO_ANIMATION_Name);

	// enable vertex array
	glEnableVertexAttribArray(0); // v
	glEnableVertexAttribArray(1); // t


	//:::::::::::::::::::::::::::::::::::::::::
	 
	// oval animation
	glGenVertexArrays(1, &VAO_POINT_Name);
	glGenBuffers(3, VBO_POINT_Name);

	// bind VAO
	glBindVertexArray(VAO_POINT_Name);

	// enable vertex array
	glEnableVertexAttribArray(0); // v
	glEnableVertexAttribArray(1); // c
	glEnableVertexAttribArray(2); // aux

	//:::::::::::::::::::::::::::::::::::::::::


	// final draw
	glGenVertexArrays(1, &VAO_FINAL_BOARD_Name);
	glGenBuffers(2, VBO_FINAL_BOARD_Name);
	glGenBuffers(1, &EBO_FINAL_BOARD_Name);

	// bind VAO
	glBindVertexArray(VAO_FINAL_BOARD_Name);

	// enable vertex array
	glEnableVertexAttribArray(0); // v
	glEnableVertexAttribArray(1); // t

	// set static vertex for FINAL board
	float RATIO = (float)WINDOW_width / (float)WINDOW_height;
	float screenSize = RATIO / 5.0;
	float left_screen_center = screenSize * -4.0;

	float incX = (screenSize*2.0) / (float)(DIVISION_X-1);
	float incY = 2.0 / (float)(DIVISION_Y-1);

	float t_incX = (1.0 / 5.0) / (float)(DIVISION_X-1);  // screen is splited into 5
	float t_incY = 1.0 / (float)(DIVISION_Y-1);


	for (int screen = 0; screen < 5; screen++)
	{
		float leftmost = left_screen_center + (screen*2.0*screenSize) - screenSize;

		for (int y = 0; y < DIVISION_Y; y++)
		{
			for (int x = 0; x < DIVISION_X; x++)
			{
				f_vert[screen][y][x][0] = leftmost + x * incX;
				f_vert[screen][y][x][1] = 1.0 - incY * y;
				f_vert[screen][y][x][2] = 0.0;
				f_vert[screen][y][x][3] = 1.0;

				f_texc[screen][y][x][0] = x * t_incX + screen*(1.0/5.0);
				f_texc[screen][y][x][1] = y * t_incY;
				f_texc[screen][y][x][2] = 0.0;
				f_texc[screen][y][x][3] = 0.0;
			}
		}
	}


	// set index array
	int num = 0;
	for (int y = 0; y < DIVISION_Y-1; y++)
	{
		// triangle strip head
		idx_vert[num] = (y+1) * DIVISION_X;
		num++;

		for (int x = 0; x < DIVISION_X; x++)
		{
			idx_vert[num] = (y + 1)*DIVISION_X + x;
			num++;
			idx_vert[num] = y * DIVISION_X + x;
			num++;
		}

		// triangle strip tail
		idx_vert[num] = y*DIVISION_X + (DIVISION_X-1);
		num++;
	}


	int bufSize = sizeof(GLfloat) * DIVISION_X * DIVISION_Y * 4;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_FINAL_BOARD_Name[0]);
	glBufferData(GL_ARRAY_BUFFER, bufSize, &f_vert[0][0][0][0], GL_DYNAMIC_DRAW); // only one screen
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_FINAL_BOARD_Name[1]);
	glBufferData(GL_ARRAY_BUFFER, bufSize, &f_texc[0][0][0][0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_FINAL_BOARD_Name);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)*NUM_IDX_ARRAY, idx_vert, GL_DYNAMIC_DRAW);


	/*
	glBindBuffer(GL_ARRAY_BUFFER, VBO_FINAL_BOARD_Name[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 4 * 4, vert, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_FINAL_BOARD_Name[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 4 * 4, texc, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	*/


	glBindVertexArray(0);
	
}