/*
 *	vector.h
 *	Header file for vector types and utilities.
 *	Based on the original work of Andre' LaMothe
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
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <math.h>

// Data types

typedef union VECTOR2D_TYPE {
	float M[2];
	struct {
		float u;
		float v;
	};
	struct {
		float x;
		float y;
	};
} VECTOR2D, *VECTOR2D_PTR;

typedef union VECTOR3D_TYPE {
	float M[3];
	struct {
		float x;
		float y;
		float z;
	};
	struct {
		float r;
		float g;
		float b;
	};
} VECTOR3D, *VECTOR3D_PTR;

typedef union VECTOR4D_TYPE {
	float M[4];
	struct {
		float x;
		float y;
		float z;
		float w;
	};
	struct {
		float r;
		float g;
		float b;
		float a;
	};
} VECTOR4D, *VECTOR4D_PTR;

// Function prototypes

void normalise (const VECTOR3D_PTR V );
float modulus(const VECTOR3D_PTR V);
float length(const VECTOR3D_PTR V);
void add (const VECTOR3D_PTR V1, const VECTOR3D_PTR V2, const VECTOR3D_PTR V3);
void sub (const VECTOR3D_PTR V1, const VECTOR3D_PTR V2, const VECTOR3D_PTR V3);
float scalar_product(const VECTOR3D_PTR V1, const VECTOR3D_PTR V2);
void cross_product (const VECTOR3D_PTR V1, const VECTOR3D_PTR V2, const VECTOR3D_PTR V3);
void cross_product_norm (const VECTOR3D_PTR V1, const VECTOR3D_PTR V2, const VECTOR3D_PTR V3);

#endif //VECTOR_H
