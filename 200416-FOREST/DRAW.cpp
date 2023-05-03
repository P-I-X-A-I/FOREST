#include "stdafx.h"
#include "mainController.h"


void mainController::draw_to_FBO()
{
	double W = (double)RENDER_width;
	double H = (double)RENDER_height;
	double view_ratio = W / H;



	// lifegame Manager *************************************
	lifegameManager_BIG_obj->loop();
	lifegameManager_obj->loop();


	// check sound ********************************************
	//sound_obj->check_sound_loop();


	// check OSC **************************************
	//big
	BOOL isBig = NOTIFICATION_CENTER_obj->isBigOn;
	if (isBig == true)
	{
		int tempIDX = rand() % 2;
		if (tempIDX == 0)
		{
			tempIDX = 0;
		}
		else
		{
			tempIDX = 6;
		}

		float xPos = (float)(rand() % 2000 - 1000)*0.001 * view_ratio;
		float yPos;
		float angle;

		int tempVal = rand() % 2;
		if (tempVal == 0)
		{
			yPos = 0.5;
			angle = 270.0 + (float)(rand() % 200 - 100)*0.01*15.0;
		}
		else
		{
			yPos = -0.5;
			angle = 90.0 + (float)(rand() % 200 - 100)*0.01*15.0;
		}

		animationManager_obj[tempIDX]->start_board(xPos, yPos, angle, 6.0, 0.8, 0.0);
		
		// restore flag
		NOTIFICATION_CENTER_obj->isBigOn = false;
	}


	// mute
	if (NOTIFICATION_CENTER_obj->dirtyFlag == true)
	{
		int isMute = OSC_hokuyo_obj->getArgAsInt(0);
		printf("get mute arg %d\n", isMute);

		// set black alpha
		act_black_alpha = 0.7;


		if (isMute < 10) // 1 or 2
		{
			isKill = true;
		}
		else // 100
		{
			isKill = false;
			lifegameManager_obj->clear_all_cell();
			lifegameManager_BIG_obj->clear_all_cell();
		}

		lifegameManager_obj->is_kill_all = isKill;
		lifegameManager_BIG_obj->is_kill_all = isKill;
		//windParticleManager_obj->is_kill_all = isKill;

		NOTIFICATION_CENTER_obj->dirtyFlag = false;
	}

	// for safety
	if (isKill == true)
	{
		mute_safety_counter++;
		if (mute_safety_counter > 600)
		{
			isKill = false;
			mute_safety_counter = 0;
			printf("mute safety counter \n");

			lifegameManager_obj->clear_all_cell();
			lifegameManager_BIG_obj->clear_all_cell();

			lifegameManager_obj->is_kill_all = isKill;
			lifegameManager_BIG_obj->is_kill_all = isKill;
			//windParticleManager_obj->is_kill_all = isKill;
		}
	}





	// check Wind *********************************************
	
	windParticleManager_obj->wind_speed = NOTIFICATION_CENTER_obj->wind_speed;
	windParticleManager_obj->wind_degree = NOTIFICATION_CENTER_obj->wind_degree;
	windParticleManager_obj->windParticle_loop();







	// bind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_toTEX_Name);


	// set viewport & clear buffer
	glViewport(0, 0, RENDER_width, RENDER_height);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);


	//////////////////////////////////////////////////////////////////////////

	// Transparency part

	act_black_alpha += (0.001 - act_black_alpha)*0.01;

	for (int i = 0; i < 4; i++)
	{
		blackV[i][2] = act_black_alpha;// use Z as alpha value
		// 1.0 ~ 0.001;
	}

	glUseProgram(TEST_PRG);

	glBindVertexArray(VAO_TEST_Name);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_TEST_Name[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, blackV, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	



	// mvp matrix


	mvp_matrix_obj->initMatrix();
	//mvp_matrix_obj->lookAt(0.0, 0.0, 1.0,
	//	0.0, 0.0, 0.0,
	//	0.0, 1.0, 0.0);
	mvp_matrix_obj->orthogonal(-view_ratio, view_ratio, // Left right
								1.0, -1.0, // botom top
								-1.0, 1.0);

	/////////////////////////////////////////////////////////////////////
	// change blend status
	glDisable(GL_DEPTH_TEST);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// change shader
	glUseProgram(ANIMATION_PRG);
	glUniformMatrix4fv(UNF_ANIMATION_MVP_MATRIX, 1, GL_FALSE, mvp_matrix_obj->getMatrix());




	//***************** DRAW TEXTURE ANIMATION BOARD *******************************************

	for (int i = 0; i < NUM_LOAD_ANIMATION; i++)
		{

		glUniform1i(UNF_ANIMATION_TEX_INDEX, i);


		animationManager_obj[i]->update_animation();

		int NUM_VERTEX = animationManager_obj[i]->num_board * 6;
		float* vPtr = animationManager_obj[i]->get_VERT_pointer();
		float* tPtr = animationManager_obj[i]->get_TEXC_pointer();


		glBindVertexArray(VAO_ANIMATION_Name);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_ANIMATION_Name[i][0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NUM_VERTEX * 4, (GLfloat*)vPtr, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_ANIMATION_Name[i][1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NUM_VERTEX * 4, (GLfloat*)tPtr, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, NUM_VERTEX);


	}// for i ( animation loop )



	// draw flying bug *******************************************************
	if (NOTIFICATION_CENTER_obj->isBugStart)
	{
		float startX = NOTIFICATION_CENTER_obj->bugValue[0];
		float startY = NOTIFICATION_CENTER_obj->bugValue[1];
		float endX = NOTIFICATION_CENTER_obj->bugValue[2];
		float endY = NOTIFICATION_CENTER_obj->bugValue[3];

		// convert
		if (startX >= 0.0)
		{
			startX = (startX - 180.0)/180.0*view_ratio;
		}
		else if (startX < 0.0)
		{
			startX = (startX + 180.0)/180.0*view_ratio;
		}

		if (endX >= 0.0)
		{
			endX = (endX - 180.) / 180.0*view_ratio;
		}
		else if (endY < 0.0)
		{
			endX = (endX + 180.0) / 180.0*view_ratio;
		}
		startY = (0.5-startY)*2.0;
		endY = (0.5-endY)*2.0;

		flyingBugManager_obj->start_bug(startX, startY, endX, endY);

		// reset flag
		NOTIFICATION_CENTER_obj->isBugStart = false;
	}

	flyingBugManager_obj->bug_loop();


	float* bugPtr_v = flyingBugManager_obj->get_bug_v_pointer();
	float* bugPtr_t = flyingBugManager_obj->get_bug_t_pointer();
	int bug_bufSize = flyingBugManager_obj->numVertex * sizeof(GLfloat) * 4;

	glUniform1i(UNF_ANIMATION_TEX_INDEX, 26);

	glBindVertexArray(VAO_ANIMATION_Name);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ANIMATION_Name[26][0]);
	glBufferData(GL_ARRAY_BUFFER, bug_bufSize, (GLfloat*)bugPtr_v, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ANIMATION_Name[26][1]);
	glBufferData(GL_ARRAY_BUFFER, bug_bufSize, (GLfloat*)bugPtr_t, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, flyingBugManager_obj->numVertex);


	
	// ********************   DRAW POINTS ****************************

	//glDisable(GL_CULL_FACE);

	// chamge shader
	glUseProgram(POINT_PRG);
	glUniformMatrix4fv(UNF_POINT_MVP_MATRIX, 1, GL_FALSE, mvp_matrix_obj->getMatrix());

	// bind vertex array
	glBindVertexArray(VAO_POINT_Name);

	GLfloat* vPtr = windParticleManager_obj->getVertPointer();
	GLfloat* cPtr = windParticleManager_obj->getColorPointer();
	GLfloat* aPtr = windParticleManager_obj->getAuxPointer();
	int bSize = sizeof(GLfloat) * windParticleManager_obj->bufSize;
		

		glBindBuffer(GL_ARRAY_BUFFER, VBO_POINT_Name[0]);
		glBufferData(GL_ARRAY_BUFFER, bSize, vPtr, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_POINT_Name[1]);
		glBufferData(GL_ARRAY_BUFFER, bSize, cPtr, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO_POINT_Name[2]);
		glBufferData(GL_ARRAY_BUFFER, bSize, aPtr, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);

		
		glDrawArrays(GL_POINTS, 0, windParticleManager_obj->num_particle);

	glBindVertexArray(0);
	
	//************************************************************


	// restore GL status
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// unbind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 
	glFlush();
}







void mainController::draw_to_FinalBoard()
{
	// set clear color
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WINDOW_width, WINDOW_height);



	//



	// init matrix
	float V_RATIO = (float)WINDOW_width / (float)WINDOW_height;
	mvp_matrix_obj->initMatrix();
	//mvp_matrix_obj->lookAt(0.0, 0.0, 1.0,
	//	0.0, 0.0, 0.0,
	//	0.0, 1.0, 0.0);
	mvp_matrix_obj->orthogonal(-V_RATIO, V_RATIO,
								-1.0, 1.0,
								-1.0, 1.0);



	// change shader
	glUseProgram(FINAL_PRG);
	glUniform1i(UNF_FINAL_TEX_INDEX, 29); // color 1
	glUniformMatrix4fv(UNF_FINAL_MVP_MATRIX, 1, GL_FALSE, mvp_matrix_obj->getMatrix());



	// set vertex

	// update vertex ( vertex is specified in setUP_VAO.m )
	glBindVertexArray(VAO_FINAL_BOARD_Name);

	//******************************************************
	//glBindBuffer(GL_ARRAY_BUFFER, VBO_FINAL_BOARD_Name[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, vert, GL_DYNAMIC_DRAW);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0); // should be called once.

	//glBindBuffer(GL_ARRAY_BUFFER, VBO_FINAL_BOARD_Name[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, texc, GL_DYNAMIC_DRAW);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0); // should be called once.

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	/////////////*********************************************************


	int bufSize = sizeof(GLfloat) * DIVISION_X * DIVISION_Y * 4;

	for (int screen = 0; screen < 5; screen++)
	{
		//update Buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO_FINAL_BOARD_Name[0]);
		glBufferData(GL_ARRAY_BUFFER, bufSize, &f_vert[screen][0][0][0], GL_DYNAMIC_DRAW); // only one screen
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		// update Buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO_FINAL_BOARD_Name[1]);
		glBufferData(GL_ARRAY_BUFFER, bufSize, &f_texc[screen][0][0][0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_FINAL_BOARD_Name);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)*NUM_IDX_ARRAY, idx_vert, GL_DYNAMIC_DRAW);

		// draw image
		glUniform1i(UNF_FINAL_DEBUG_FLAG, 0);
		glDrawElements(GL_TRIANGLE_STRIP, NUM_IDX_ARRAY, GL_UNSIGNED_INT, 0);
		// draw cariblation grid
		//glUniform1i(UNF_FINAL_DEBUG_FLAG, screen+1);
		//glDrawElements(GL_POINTS, NUM_IDX_ARRAY, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);

	// display
	SwapBuffers(OPENGL_INITIALIZER_obj->OpenGL_HDC);
	glFlush();

}