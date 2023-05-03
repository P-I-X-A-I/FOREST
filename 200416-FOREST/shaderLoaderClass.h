#pragma once

#include "glew.h"
#include "wglew.h"

#include <gl\GL.h> // in system folder
#include <gl\GLU.h>// in system folder

class shaderLoaderClass
{
public:




	shaderLoaderClass();
	~shaderLoaderClass();


	bool loadShaderSource_And_CompileShader(const char* name, int type, GLuint* toObj);
	bool loadShaderSource_And_CompileShader_fromPtr(const char* stringPtr, int type, GLuint* toObj);
	bool createProgram_And_AttachShader(GLuint* prgPtr, GLuint* vsPtr, GLuint* gsPtr, GLuint* fsPtr );

	void getUniformLocation(GLuint* prgObj, GLint* uniform, const char* name);
};

