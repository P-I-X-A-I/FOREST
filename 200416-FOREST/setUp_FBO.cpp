#include "stdafx.h"
#include "mainController.h"


void mainController::setUp_FBO()
{
	printf("\n *** SETUP FBO *** \n");

	// generate FBO
	glGenFramebuffers(1, &FBO_toTEX_Name);

	// generate texture for FBO
	glGenTextures(3, TEX_forFBO_Name);

	// bind texture for FBO ( 29, 30, 31 )
	// color 0
	glActiveTexture(GL_TEXTURE29);
	glBindTexture(GL_TEXTURE_2D, TEX_forFBO_Name[0]);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		RENDER_width,
		RENDER_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		nullptr);

	// color 1 ( as 32Float )
	/*
	glActiveTexture(GL_TEXTURE30);
	glBindTexture(GL_TEXTURE_2D, TEX_forFBO_Name[1]);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA32F,  // GL_RGB16F, GL_RGB32F are also for COLOR attachment, but RGBA32F is required format 
		RENDER_width,
		RENDER_height,
		0,
		GL_RGBA,
		GL_FLOAT,
		nullptr);

		*/
	// depth
	glActiveTexture(GL_TEXTURE31);
	glBindTexture(GL_TEXTURE_2D, TEX_forFBO_Name[2]);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_DEPTH_COMPONENT16,
		RENDER_width,
		RENDER_height,
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		nullptr);

	glActiveTexture(0);



	// Attach these textures to FBO

	glBindFramebuffer(GL_FRAMEBUFFER, FBO_toTEX_Name);

	// color 0
	glFramebufferTexture2D(GL_FRAMEBUFFER,
							GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_2D,
							TEX_forFBO_Name[0],
							0);

	/*
	// color 1 ( as float )
	glFramebufferTexture2D(GL_FRAMEBUFFER,
							GL_COLOR_ATTACHMENT1,
							GL_TEXTURE_2D,
							TEX_forFBO_Name[1],
							0);
	*/
	// depth
	glFramebufferTexture2D(GL_FRAMEBUFFER,
							GL_DEPTH_ATTACHMENT,
							GL_TEXTURE_2D,
							TEX_forFBO_Name[2],
							0);

	// setup drawbuffer
	//GLenum bufs[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	GLenum bufs[1] = { GL_COLOR_ATTACHMENT0 };
	//glDrawBuffers(2, bufs);
	glDrawBuffers(1, bufs);


	// check for status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	printf("FBO status %x (8cd5 is OK)\n", status);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}