/*
 * shaders.c - utilities for loading and compiling shaders
 *
 * Simple test program for OpenGL and glX
 * Copyright (c) 2007 Cesare Tirabassi <norsetto@ubuntu.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "shaders.h"

/*
 * Load shader from disk into a null-terminated string
 *
 */

GLubyte *LoadShaderText(GLuint *shaderLength, const char *fileName)
{
GLubyte *shaderText = NULL;
FILE *fp;

	*shaderLength = 0;
	// Open shader file
	fp = fopen(fileName, "r");
	
	if (!fp)
	{
		printf("<!> Cannot open shader file  \"%s\"\n", fileName);
		return NULL;
	}

	// Compute shader lenght
	while (fgetc(fp) != EOF)
		++(*shaderLength);

	rewind(fp);

	shaderText = (GLubyte *)malloc(*shaderLength+1);

	if (*shaderLength)
		fread(shaderText, 1, *shaderLength, fp);

	shaderText[*shaderLength] = '\0';

	fclose(fp);
	return shaderText;
}

/*
 * Compile low-level shader
 *
 */

GLboolean CompileShader(GLenum Program_type, GLuint id, const char *fileName)
{
GLubyte *fsString = NULL;
GLuint fsLength;
GLint errorPos;

	// Create low-level shader object and specify shader text

	fsString = LoadShaderText( &fsLength, fileName);
	if (!fsString)
	{
		printf("<!> Unable to load \"%s\"\n", fileName);
		return GL_FALSE;
	}

	glBindProgramARB(Program_type, id);
	glProgramStringARB(Program_type, GL_PROGRAM_FORMAT_ASCII_ARB, fsLength, fsString);
	
	if (fsString)
		free(fsString);

	glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errorPos);

	if (errorPos != -1)
	{
		printf("<!> Error in low-level shader \"%s\" at position %d\n", fileName, errorPos);
		printf("    Error string: %s\n", glGetString(GL_PROGRAM_ERROR_STRING_ARB));
		return GL_FALSE;
	}

	printf("<-> Compiled succesfully shader in \"%s\"\n", fileName);
	return GL_TRUE;
}

/*
 * Compile GLSL shader
 *
 */

GLboolean CompileGLSLshaders( GLhandleARB *progObj, const char *vertexFile, const char *fragmentFile, GLboolean validate )
{
GLhandleARB vShader, fShader;
GLcharARB *vsStringPtr[1];
GLcharARB *fsStringPtr[1];
GLuint fsLength;
GLint success;

	// Create shader objects and specify shader text
	vShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	fShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	vsStringPtr[0] = (char *)LoadShaderText(&fsLength, vertexFile);
	glShaderSourceARB(vShader, 1, (const char **)vsStringPtr, NULL);
	fsStringPtr[0] = (char *)LoadShaderText(&fsLength, fragmentFile);
	glShaderSourceARB(fShader, 1, (const char **)fsStringPtr, NULL);

	// Compile shaders and check for any errors
	glCompileShaderARB(vShader);
	glGetObjectParameterivARB(vShader, GL_OBJECT_COMPILE_STATUS_ARB, &success);
	if (!success)
	{
		GLbyte infoLog[MAX_INFO_LOG_SIZE];
		glGetInfoLogARB(vShader, MAX_INFO_LOG_SIZE, NULL, (char *)infoLog);
		printf("<!> Error in vertex shader compilation\n");
		printf("Info log: %s\n", infoLog);
		return GL_FALSE;
	}

	glCompileShaderARB(fShader);
	glGetObjectParameterivARB(fShader, GL_OBJECT_COMPILE_STATUS_ARB, &success);
	if (!success)
	{
		GLbyte infoLog[MAX_INFO_LOG_SIZE];
		glGetInfoLogARB(fShader, MAX_INFO_LOG_SIZE, NULL, (char *)infoLog);
		printf("<!> Error in fragment shader compilation\n");
		printf("Info log: %s\n", infoLog);
		return GL_FALSE;
	}

	// Create program object, attach shaders, then link
	glAttachObjectARB(*progObj, vShader);
	glAttachObjectARB(*progObj, fShader);

	glLinkProgramARB(*progObj);
	glGetObjectParameterivARB(*progObj, GL_OBJECT_LINK_STATUS_ARB, &success);
	if (!success)
	{
		GLbyte infoLog[MAX_INFO_LOG_SIZE];
		glGetInfoLogARB(*progObj, MAX_INFO_LOG_SIZE, NULL, (char *)infoLog);
		printf("<!> Error in program linkage\n");
		printf("Info log: %s\n", infoLog);
		return GL_FALSE;
	}

	// Validate shader program
	if (validate)
	{
		glValidateProgramARB(*progObj);
		glGetObjectParameterivARB(*progObj, GL_OBJECT_VALIDATE_STATUS_ARB, &success);
		if (!success)
		{
			GLbyte infoLog[MAX_INFO_LOG_SIZE];
			glGetInfoLogARB(*progObj, MAX_INFO_LOG_SIZE, NULL, (char *)infoLog);
			printf("<!> Error in program validation\n");
			printf("Info log: %s\n", infoLog);
			return GL_FALSE;
		}
	}

	glDeleteObjectARB(vShader);
	glDeleteObjectARB(fShader);

	printf("<-> Compiled succesfully GLSL shader in \"%s\"\n", vertexFile);
	printf("<-> Compiled succesfully GLSL shader in \"%s\"\n", fragmentFile);

	return GL_TRUE;
}
