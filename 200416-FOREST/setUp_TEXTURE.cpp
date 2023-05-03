#include "stdafx.h"
#include "mainController.h"

void mainController::setUp_TEXTURE()
{
	printf("\n *** SETUP TEXTURE *** \n");

	// generate sampler
	glGenSamplers(1, &Sampler_Name);

	// setup sampler
	glSamplerParameteri(Sampler_Name, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(Sampler_Name, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(Sampler_Name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(Sampler_Name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// set sampler to all texture unit
	GLint maxTex;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTex);

	for (int i = 0; i < maxTex; i++)
	{
		glBindSampler(i, Sampler_Name);
	}


	// Generate Texture Name
	glGenTextures(NUM_TEXTURE_IMG, TEX_Name);


	// load texture data
	// texture 0 *****************************************
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[0]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/A01_600x856_w20h14_280f.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);

	// texture 1 *******************************************
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[1]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim02.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);

	// texture 2 *******************************************
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[2]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim03.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);

	// texture 3 *******************************************
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[3]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim04.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);

	// texture 4 *******************************************
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[4]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim05.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 5 *******************************************
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[5]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim06.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 6 *******************************************
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[6]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/A07_630x1200_w21h11_220f.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 7 *******************************************
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[7]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim08.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 8 *******************************************
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[8]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim09.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 9 *******************************************
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[9]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim10.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 10 *******************************************
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[10]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim11.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 11 *******************************************
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[11]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim12.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 12 *******************************************
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[12]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim13.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 13 *******************************************
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[13]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim14.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 14 *******************************************
	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[14]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim15.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 15 *******************************************
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[15]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim16.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 16 *******************************************
	glActiveTexture(GL_TEXTURE16);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[16]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim17.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 17 *******************************************
	glActiveTexture(GL_TEXTURE17);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[17]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim18.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 18 *******************************************
	glActiveTexture(GL_TEXTURE18);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[18]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim19.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 19 *******************************************
	glActiveTexture(GL_TEXTURE19);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[19]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim20.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 20 *******************************************
	glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[20]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim21.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 21 *******************************************
	glActiveTexture(GL_TEXTURE21);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[21]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim22.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 22 *******************************************
	glActiveTexture(GL_TEXTURE22);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[22]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim23.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 23 *******************************************
	glActiveTexture(GL_TEXTURE23);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[23]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim24.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 24 *******************************************
	glActiveTexture(GL_TEXTURE24);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[24]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim25.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 25 *******************************************
	glActiveTexture(GL_TEXTURE25);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[25]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim26.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// texture 26 *******************************************
	glActiveTexture(GL_TEXTURE26);
	glBindTexture(GL_TEXTURE_2D, TEX_Name[26]);

	PNG24_LOADER_obj->load_png24_from_path("PNG/anim_kiki02_450x270_12.png");

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		PNG24_LOADER_obj->PNG_width,
		PNG24_LOADER_obj->PNG_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		PNG24_LOADER_obj->PNG_dataPtr);
	// ***
	// ***
	// ***
	// ***
}