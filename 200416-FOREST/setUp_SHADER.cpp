#include "stdafx.h"
#include "mainController.h"


void mainController::setUp_SHADER()
{
	printf("\n *** SETUP SHADER *** \n");


	// setup test shader
	SHADER_LOADER_obj->loadShaderSource_And_CompileShader("SHADER/TEST_VS.txt", 0, &TEST_VS);
	SHADER_LOADER_obj->loadShaderSource_And_CompileShader("SHADER/TEST_FS.txt", 2, &TEST_FS);
	SHADER_LOADER_obj->createProgram_And_AttachShader(&TEST_PRG, &TEST_VS, NULL, &TEST_FS);

	//SHADER_LOADER_obj->getUniformLocation(&TEST_PRG, &UNF_TEST_MVP_MATRIX, "MVP_MATRIX");


	//::::::::::::::::::::::::::::::::::::::::::::

	SHADER_LOADER_obj->loadShaderSource_And_CompileShader("SHADER/ANIMATION_VS.txt", 0, &ANIMATION_VS);
	SHADER_LOADER_obj->loadShaderSource_And_CompileShader("SHADER/ANIMATION_FS.txt", 2, &ANIMATION_FS);
	SHADER_LOADER_obj->createProgram_And_AttachShader(&ANIMATION_PRG, &ANIMATION_VS, NULL, &ANIMATION_FS);

	SHADER_LOADER_obj->getUniformLocation(&ANIMATION_PRG, &UNF_ANIMATION_MVP_MATRIX, "MVP_MATRIX");
	SHADER_LOADER_obj->getUniformLocation(&ANIMATION_PRG, &UNF_ANIMATION_TEX_INDEX, "TEX_INDEX");
	//::::::::::::::::::::::::::::::::::::::::::::

	SHADER_LOADER_obj->loadShaderSource_And_CompileShader("SHADER/POINT_VS.txt", 0, &POINT_VS);
	SHADER_LOADER_obj->loadShaderSource_And_CompileShader("SHADER/POINT_FS.txt", 2, &POINT_FS);
	SHADER_LOADER_obj->createProgram_And_AttachShader(&POINT_PRG, &POINT_VS, NULL, &POINT_FS);

	SHADER_LOADER_obj->getUniformLocation(&POINT_PRG, &UNF_POINT_MVP_MATRIX, "MVP_MATRIX");

	//::::::::::::::::::::::::::::::::::::::::::::

	SHADER_LOADER_obj->loadShaderSource_And_CompileShader("SHADER/FINAL_VS.txt", 0, &FINAL_VS);
	SHADER_LOADER_obj->loadShaderSource_And_CompileShader("SHADER/FINAL_FS.txt", 2, &FINAL_FS);
	SHADER_LOADER_obj->createProgram_And_AttachShader(&FINAL_PRG, &FINAL_VS, NULL, &FINAL_FS);

	SHADER_LOADER_obj->getUniformLocation(&FINAL_PRG, &UNF_FINAL_MVP_MATRIX, "MVP_MATRIX");
	SHADER_LOADER_obj->getUniformLocation(&FINAL_PRG, &UNF_FINAL_TEX_INDEX, "TEX_INDEX");
	SHADER_LOADER_obj->getUniformLocation(&FINAL_PRG, &UNF_FINAL_DEBUG_FLAG, "DEBUG_FLAG");
}