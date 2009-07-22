/*
 * shaders.h - header file for shaders.c
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
 *
 */

#ifndef SHADERS_H
#define SHADERS_H

#include <GL/gl.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define MAX_INFO_LOG_SIZE 2048

//Function prototypes
GLubyte  *LoadShaderText( GLuint *shaderLength, const char *fileName );
GLboolean CompileShader( GLenum Program_type, GLuint id, const char *fileName );
GLboolean CompileGLSLshaders( GLhandleARB *progObj, const char *vertexFile, const char *fragmentFile, GLboolean validate );

#endif //SHADERS_H
