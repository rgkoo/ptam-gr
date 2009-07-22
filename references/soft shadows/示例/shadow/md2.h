/*
 *	md2.h
 *	Header file for md2 file utilities.
 *	Based on the original work of www.paulsprojects.net and Andre' LaMothe
 *
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

#ifndef MD2_H
#define MD2_H

#include <GL/gl.h>
#include <GL/glext.h>
#include <malloc.h>
#include <string.h>
#include <math.h>

#include "image.h"
#include "vector.h"
#include "common.h"

// Constants

//MD2 FILE IDENTIFIER = IDP2
#define MD2_IDENTIFIER (('I') + ('D' << 8) + ('P' << 16) + ('2' << 24))

//MD2 FILE VERSION = 8
#define MD2_VERSION 8

//MD2 animation frames
static const int anim_begin[] = { 0, 40, 46, 54, 58, 62, 66, 72, 84,  95, 112, 123, 135, 154,
							160, 169, 173, 178, 184, 190 };
static const int anim_end[]   = { 39, 45, 53, 57, 61, 65, 71, 83, 94, 111, 122, 134, 153, 159,
							168, 172, 177, 183, 189, 197 };

//Normals used in the MD2 file format
static const GLfloat NORMALS[486] =
	{-0.525731f,  0.000000f,  0.850651f,
	 -0.442863f,  0.238856f,  0.864188f,
	 -0.295242f,  0.000000f,  0.955423f,
	 -0.309017f,  0.500000f,  0.809017f,
	 -0.162460f,  0.262866f,  0.951056f,
	  0.000000f,  0.000000f,  1.000000f,
	  0.000000f,  0.850651f,  0.525731f,
	 -0.147621f,  0.716567f,  0.681718f,
	  0.147621f,  0.716567f,  0.681718f,
	  0.000000f,  0.525731f,  0.850651f,
	  0.309017f,  0.500000f,  0.809017f,
	  0.525731f,  0.000000f,  0.850651f,
	  0.295242f,  0.000000f,  0.955423f,
	  0.442863f,  0.238856f,  0.864188f,
	  0.162460f,  0.262866f,  0.951056f,
	 -0.681718f,  0.147621f,  0.716567f,
	 -0.809017f,  0.309017f,  0.500000f,
	 -0.587785f,  0.425325f,  0.688191f,
	 -0.850651f,  0.525731f,  0.000000f,
	 -0.864188f,  0.442863f,  0.238856f,
	 -0.716567f,  0.681718f,  0.147621f,
	 -0.688191f,  0.587785f,  0.425325f,
	 -0.500000f,  0.809017f,  0.309017f,
	 -0.238856f,  0.864188f,  0.442863f,
	 -0.425325f,  0.688191f,  0.587785f,
	 -0.716567f,  0.681718f, -0.147621f,
	 -0.500000f,  0.809017f, -0.309017f,
	 -0.525731f,  0.850651f,  0.000000f,
	  0.000000f,  0.850651f, -0.525731f,
	 -0.238856f,  0.864188f, -0.442863f,
	  0.000000f,  0.955423f, -0.295242f,
	 -0.262866f,  0.951056f, -0.162460f,
	  0.000000f,  1.000000f,  0.000000f,
	  0.000000f,  0.955423f,  0.295242f,
	 -0.262866f,  0.951056f,  0.162460f,
	  0.238856f,  0.864188f,  0.442863f,
	  0.262866f,  0.951056f,  0.162460f,
	  0.500000f,  0.809017f,  0.309017f,
	  0.238856f,  0.864188f, -0.442863f,
	  0.262866f,  0.951056f, -0.162460f,
	  0.500000f,  0.809017f, -0.309017f,
	  0.850651f,  0.525731f,  0.000000f,
	  0.716567f,  0.681718f,  0.147621f,
	  0.716567f,  0.681718f, -0.147621f,
	  0.525731f,  0.850651f,  0.000000f,
	  0.425325f,  0.688191f,  0.587785f,
	  0.864188f,  0.442863f,  0.238856f,
	  0.688191f,  0.587785f,  0.425325f,
	  0.809017f,  0.309017f,  0.500000f,
	  0.681718f,  0.147621f,  0.716567f,
	  0.587785f,  0.425325f,  0.688191f,
	  0.955423f,  0.295242f,  0.000000f,
	  1.000000f,  0.000000f,  0.000000f,
	  0.951056f,  0.162460f,  0.262866f,
	  0.850651f, -0.525731f,  0.000000f,
	  0.955423f, -0.295242f,  0.000000f,
	  0.864188f, -0.442863f,  0.238856f,
	  0.951056f, -0.162460f,  0.262866f,
	  0.809017f, -0.309017f,  0.500000f,
	  0.681718f, -0.147621f,  0.716567f,
	  0.850651f,  0.000000f,  0.525731f,
	  0.864188f,  0.442863f, -0.238856f,
	  0.809017f,  0.309017f, -0.500000f,
	  0.951056f,  0.162460f, -0.262866f,
	  0.525731f,  0.000000f, -0.850651f,
	  0.681718f,  0.147621f, -0.716567f,
	  0.681718f, -0.147621f, -0.716567f,
	  0.850651f,  0.000000f, -0.525731f,
	  0.809017f, -0.309017f, -0.500000f,
	  0.864188f, -0.442863f, -0.238856f,
	  0.951056f, -0.162460f, -0.262866f,
	  0.147621f,  0.716567f, -0.681718f,
	  0.309017f,  0.500000f, -0.809017f,
	  0.425325f,  0.688191f, -0.587785f,
	  0.442863f,  0.238856f, -0.864188f,
	  0.587785f,  0.425325f, -0.688191f,
	  0.688191f,  0.587785f, -0.425325f,
	 -0.147621f,  0.716567f, -0.681718f,
	 -0.309017f,  0.500000f, -0.809017f,
	  0.000000f,  0.525731f, -0.850651f,
	 -0.525731f,  0.000000f, -0.850651f,
	 -0.442863f,  0.238856f, -0.864188f,
	 -0.295242f,  0.000000f, -0.955423f,
	 -0.162460f,  0.262866f, -0.951056f,
	  0.000000f,  0.000000f, -1.000000f,
	  0.295242f,  0.000000f, -0.955423f,
	  0.162460f,  0.262866f, -0.951056f,
	 -0.442863f, -0.238856f, -0.864188f,
	 -0.309017f, -0.500000f, -0.809017f,
	 -0.162460f, -0.262866f, -0.951056f,
	  0.000000f, -0.850651f, -0.525731f,
	 -0.147621f, -0.716567f, -0.681718f,
	  0.147621f, -0.716567f, -0.681718f,
	  0.000000f, -0.525731f, -0.850651f,
	  0.309017f, -0.500000f, -0.809017f,
	  0.442863f, -0.238856f, -0.864188f,
	  0.162460f, -0.262866f, -0.951056f,
	  0.238856f, -0.864188f, -0.442863f,
	  0.500000f, -0.809017f, -0.309017f,
	  0.425325f, -0.688191f, -0.587785f,
	  0.716567f, -0.681718f, -0.147621f,
	  0.688191f, -0.587785f, -0.425325f,
	  0.587785f, -0.425325f, -0.688191f,
	  0.000000f, -0.955423f, -0.295242f,
	  0.000000f, -1.000000f,  0.000000f,
	  0.262866f, -0.951056f, -0.162460f,
	  0.000000f, -0.850651f,  0.525731f,
	  0.000000f, -0.955423f,  0.295242f,
	  0.238856f, -0.864188f,  0.442863f,
	  0.262866f, -0.951056f,  0.162460f,
	  0.500000f, -0.809017f,  0.309017f,
	  0.716567f, -0.681718f,  0.147621f,
	  0.525731f, -0.850651f,  0.000000f,
	 -0.238856f, -0.864188f, -0.442863f,
	 -0.500000f, -0.809017f, -0.309017f,
	 -0.262866f, -0.951056f, -0.162460f,
	 -0.850651f, -0.525731f,  0.000000f,
	 -0.716567f, -0.681718f, -0.147621f,
	 -0.716567f, -0.681718f,  0.147621f,
	 -0.525731f, -0.850651f,  0.000000f,
	 -0.500000f, -0.809017f,  0.309017f,
	 -0.238856f, -0.864188f,  0.442863f,
	 -0.262866f, -0.951056f,  0.162460f,
	 -0.864188f, -0.442863f,  0.238856f,
	 -0.809017f, -0.309017f,  0.500000f,
	 -0.688191f, -0.587785f,  0.425325f,
	 -0.681718f, -0.147621f,  0.716567f,
	 -0.442863f, -0.238856f,  0.864188f,
	 -0.587785f, -0.425325f,  0.688191f,
	 -0.309017f, -0.500000f,  0.809017f,
	 -0.147621f, -0.716567f,  0.681718f,
	 -0.425325f, -0.688191f,  0.587785f,
	 -0.162460f, -0.262866f,  0.951056f,
	  0.442863f, -0.238856f,  0.864188f,
	  0.162460f, -0.262866f,  0.951056f,
	  0.309017f, -0.500000f,  0.809017f,
	  0.147621f, -0.716567f,  0.681718f,
	  0.000000f, -0.525731f,  0.850651f,
	  0.425325f, -0.688191f,  0.587785f,
	  0.587785f, -0.425325f,  0.688191f,
	  0.688191f, -0.587785f,  0.425325f,
	 -0.955423f,  0.295242f,  0.000000f,
	 -0.951056f,  0.162460f,  0.262866f,
	 -1.000000f,  0.000000f,  0.000000f,
	 -0.850651f,  0.000000f,  0.525731f,
	 -0.955423f, -0.295242f,  0.000000f,
	 -0.951056f, -0.162460f,  0.262866f,
	 -0.864188f,  0.442863f, -0.238856f,
	 -0.951056f,  0.162460f, -0.262866f,
	 -0.809017f,  0.309017f, -0.500000f,
	 -0.864188f, -0.442863f, -0.238856f,
	 -0.951056f, -0.162460f, -0.262866f,
	 -0.809017f, -0.309017f, -0.500000f,
	 -0.681718f,  0.147621f, -0.716567f,
	 -0.681718f, -0.147621f, -0.716567f,
	 -0.850651f,  0.000000f, -0.525731f,
	 -0.688191f,  0.587785f, -0.425325f,
	 -0.587785f,  0.425325f, -0.688191f,
	 -0.425325f,  0.688191f, -0.587785f,
	 -0.425325f, -0.688191f, -0.587785f,
	 -0.587785f, -0.425325f, -0.688191f,
	 -0.688191f, -0.587785f, -0.425325f};

// Data types

//MD2 HEADER structure format
typedef struct MD2_HEADER_TYPE
{
	int identifier;		//"IDP2"
	int version;		//8
	int skinWidth, skinHeight; //width and height of texture maps used for skinning
	int frameSize;		//bytes per frame of animation
	int numSkins;		//total number of skins listed by ASCII filenames
	int numVertices;	//number of vertices in each model frame
	int numTextcoords;	//number of texture coordinates in entire file
	int numPolys;		//number of polygons per model
	int numGLCommands;	//number of openGL commands
	int numFrames;		//number of animation frames

	//offsets in file
	int offsetSkins;
	int offsetTextcoords;
	int offsetPolys;
	int offsetFrames;
	int offsetGLCommands;
	int offsetEnd;
} MD2_HEADER, *MD2_HEADER_PTR;

//MD2 animation states
typedef enum MD2_ANIMATION_TYPE 
{
	ANIMATION_STANDING_IDLE,	// model standing and idling
	ANIMATION_RUN,			// model running
	ANIMATION_ATTACK,		// model attacking
	ANIMATION_PAIN_1,		// model being hit (1)
	ANIMATION_PAIN_2,		//   "       "     (2)
	ANIMATION_PAIN_3,		//   "       "     (3)
	ANIMATION_JUMP,			// model jumping
	ANIMATION_FLIP,			// model "using hand gestures !?"
	ANIMATION_SALUTE,		// model saluting
	ANIMATION_TAUNT,		// model taunting
	ANIMATION_WAVE,			// model waving at someone
	ANIMATION_POINT,		// model pointing at someone
	ANIMATION_CROUCH_IDLE,		// model crouching and idling
	ANIMATION_CROUCH_WALK,		// model walking while crouching
	ANIMATION_CROUCH_ATTACK,	// model attacking while crouching
	ANIMATION_CROUCH_PAIN,		// model being hit while crouching
	ANIMATION_CROUCH_DEATH,		// model dying while crouching
	ANIMATION_DEATH_FORWARD,	// model dying while falling backward
	ANIMATION_DEATH_BACKWARD,	// model dying while falling forward
	ANIMATION_DEATH_SLOW		// model dying slowly (any direction)

} MD2_ANIMATION_STATE;

//MD2 animation commands
typedef enum MD2_COMMANDS_TYPE 
{
	ANIMATION_START,
	ANIMATION_PAUSE,
	ANIMATION_CHANGE,
	ANIMATION_CONTINUE

} MD2_ANIMATION_COMMAND;

//Single u,v texture format
typedef struct MD2_TEXTCOORD_TYPE
{
	short u,v;
} MD2_TEXTCOORD, *MD2_TEXTCOORD_PTR;

//Single MD2 polygon (triangle) format
typedef struct MD2_POLY_TYPE
{
	unsigned short vindex[3];	//Vertex indices
	unsigned short tindex[3];	//Texture indices
} MD2_POLY, *MD2_POLY_PTR;

//Single MD2 point format, contains:
//	an 8-bit scaled x, y, z
//	an index for the normal (to a normal table)
typedef struct MD2_POINT_TYPE
{
	unsigned char v[3];		//vertex x, y, z coordinates in compressed format
	unsigned char normal_index;	//index to normal table
} MD2_POINT, *MD2_POINT_PTR;

//Single MD2 frame format
typedef struct MD2_FRAME_TYPE
{
	float scale[3];		//x, y, z scaling factors
	float translate[3];	//x, y, z translation factors
	char name[16];		//ASCII name of model
	MD2_POINT vlist[1];	//pointer to vertex storage array
} MD2_FRAME, *MD2_FRAME_PTR;

//MD2 model (to be used for rendering)
typedef struct MD2_MODEL_TYPE
{
	//Geometric variables
	VECTOR3D position;
	float rotation;		//Radians around the +YaxisPosition
	VECTOR3D color;		//Base color

	int num_frames;		//Number of frames in the model
	int num_polys;		//Number of polygons in the model
	int num_verts;		//Number of vertices in the model
	int num_textcoords;	//Number of texture coordinates in the model
	
	MD2_POLY_PTR polys;	//Pointer to polygon list
	VECTOR3D_PTR vlist;	//Pointer to vertex coordinates list
	VECTOR2D_PTR tlist;	//Pointer to texture coordinates list
	VECTOR3D_PTR normlist;	//Pointer to polygon normals list
	VECTOR3D_PTR tanglist;	//Pointer to polygon tangents list
	VECTOR3D_PTR binolist;	//Pointer to polygon binormals list

	//GL commands
	int *GL_commands;	//Pointer to OpenGL commands list

	//Textures
	IMAGE_PTR skin;
	IMAGE_PTR normal;

	GLint tangent;		//Adress of the uniform variable
	GLint binormal;		//Adress of the uniform variable

	//Animation variables
	MD2_ANIMATION_STATE anim_state;
	MD2_ANIMATION_STATE current_animation;
	MD2_ANIMATION_COMMAND anim_command;
	GLdouble anim_time;	//From 0 to sec_per_frame, count the time in between frames
	GLdouble sec_per_frame;	//The inverse is how many frames per seconds we display
	int anim_counter;
	int anim_jump;
	GLboolean anim_start;
	GLboolean change_animation;

	//Interpolation variables
	VECTOR3D old_frameV;
	VECTOR3D new_frameV;
	VECTOR3D old_frameN;
	VECTOR3D new_frameN;
	VECTOR3D old_frameT;
	VECTOR3D new_frameT;
	VECTOR3D old_frameB;
	VECTOR3D new_frameB;

	//Kinematic variables
	GLfloat delta_rate;	//rotational rate (deg/s)
	GLfloat delta_x;	//Translational speed (along x)
	GLfloat delta_z;	//Translational speed (along z)

} MD2_MODEL, *MD2_MODEL_PTR;

// Function prototypes
GLboolean LoadMD2( MD2_MODEL_PTR md2_model, const char * filename );
void UpdateMD2(MD2_MODEL_PTR md2_model, GLdouble dt );
void AnimateMD2( MD2_MODEL_PTR md2_model, GLdouble dt );
void DrawMD2( MD2_MODEL_PTR md2_model );
#endif	//MD2_H
