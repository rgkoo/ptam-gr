/*
 * extension.c - utilities to detect opengl extensions and version
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

#include "extension.h"

/*
 * Parse GL_VERSION and return the major and minor numbers
 *
 */

GLboolean getGLversion( GLuint *major, GLuint *minor )
{
const char* version;

	version = (const char*)glGetString( GL_VERSION );

	if (version != NULL)
		if ( sscanf( version, "%d.%d", major, minor ) == 2 )
			return GL_TRUE;

	printf( "<!>Invalid GL_VERSION format\n" );
	return GL_FALSE;
}


/*
 * Parse GL_SHADING_LANGUAGE_VERSION and return the major and minor numbers
 *
 */

GLboolean getGLSLversion( GLuint *major, GLuint *minor )
{
GLuint gl_major, gl_minor;

	if (!getGLversion( &gl_major, &gl_minor ))
		return GL_FALSE;

	if( gl_major == 1 )
	{
		/* GL v1.x can only provide GLSL v1.00 as an extension */
		if ( IsExtSupported("GL_ARB_shading_language_100") )
		{
			*major = 1;
			*minor = 0;
			return GL_TRUE;
		}
		else
		{
			printf( "<!>ARB shading language 100 extension not supported\n" );
			return GL_FALSE;
		}
	}
	else if( gl_major >= 2 )
	{
		/* GL v2.0 and greater must parse the version string */
		const char* version = (const char*)glGetString( GL_SHADING_LANGUAGE_VERSION );
		if (version != NULL)
			if ( sscanf( version, "%d.%d", major, minor ) == 2 )
				return GL_TRUE;

		printf( "<!>Invalid GL_SHADING_LANGUAGE_VERSION format\n" );
	}

	return GL_FALSE;
}

/*
 * This function determines if the named OpenGL Extension is supported
 *
 */

GLboolean IsExtSupported(const char *extension)
{
GLubyte *extensions = NULL;
const GLubyte *start;
GLubyte *where, *terminator;

	where = (GLubyte *) strchr(extension, ' ');
	if (where || *extension == '\0')
		return GL_FALSE;

	extensions = (GLubyte *)glGetString(GL_EXTENSIONS);

	start = extensions;
	for (;;) 
		{
		where = (GLubyte *) strstr((const char *) start, extension);

		if (!where)
			break;

		terminator = where + strlen(extension);

		if (where == start || *(where - 1) == ' ')
			{
			if (*terminator == ' ' || *terminator == '\0')
				return GL_TRUE;
			}
		start = terminator;
		}
	return GL_FALSE;
}
