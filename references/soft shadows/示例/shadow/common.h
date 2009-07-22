/*
 * common.h - header file with common definitions
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

#ifndef COMMON_H
#define COMMON_H

// use this to delete an array
#define SAFE_DELETE_ARRAY(p)  \
  if(p) { \
    free (p); \
    (p) = 0; \
  } \

//Random number generators

#define RandZeroToOne      ((float)rand()/RAND_MAX)
#define RandMinusOneToOne  (RandZeroToOne*2.0f-1.0f)

//Set offset i of a generic buffer
#define BUFFER_OFFSET(i) ((char *)NULL+(i))

// Define trigonometric constants (PI/2, PI, 3/2 PI, PI/3, 2 PI, 180/PI, PI/180)
#define PIo2 1.570796f
#define PI   3.141593f
#define PI32 4.712389f
#define PIo3 1.047198f
#define PI2  6.283185f
#define rad2deg 57.2957795f
#define deg2rad 0.01745329f

#endif //COMMON_H
