/*
 * vector.c - utilities for vector math
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

#include <math.h>
#include "vector.h"

void normalise (const VECTOR3D_PTR V )
{
float rsqrt;

	rsqrt = (float) sqrtf(V->x * V->x + V->y * V->y + V->z * V->z);
	if (rsqrt)
		rsqrt = 1.0f / rsqrt;
	V->x *= rsqrt; V->y *= rsqrt; V->z *= rsqrt;
}

float modulus(const VECTOR3D_PTR V)
{
	return ( V->x * V->x + V->y * V->y + V->z * V->z );
}

float length(const VECTOR3D_PTR V)
{
	return sqrtf( V->x * V->x + V->y * V->y + V->z * V->z );
}

float scalar_product(const VECTOR3D_PTR V1, const VECTOR3D_PTR V2)
{
	return ( V1->x * V2->x + V1->y * V2->y + V1->z * V2->z );
}

void add (const VECTOR3D_PTR V1, const VECTOR3D_PTR V2, const VECTOR3D_PTR V3)
{
	V3->x = V1->x + V2->x;
	V3->y = V1->y + V2->y;
	V3->z = V1->z + V2->z;
}

void sub (const VECTOR3D_PTR V1, const VECTOR3D_PTR V2, const VECTOR3D_PTR V3)
{
	V3->x = V1->x - V2->x;
	V3->y = V1->y - V2->y;
	V3->z = V1->z - V2->z;
}

void cross_product (const VECTOR3D_PTR V1, const VECTOR3D_PTR V2, const VECTOR3D_PTR V3)
{
	V3->x = V1->y * V2->z - V1->z * V2->y;
	V3->y = V1->z * V2->x - V1->x * V2->z;
	V3->z = V1->x * V2->y - V1->y * V2->x;
}

void cross_product_norm (const VECTOR3D_PTR V1, const VECTOR3D_PTR V2, const VECTOR3D_PTR V3)
{
float rsqrt;

	V3->x = V1->y * V2->z - V1->z * V2->y;
	V3->y = V1->z * V2->x - V1->x * V2->z;
	V3->z = V1->x * V2->y - V1->y * V2->x;

	rsqrt = (float) sqrtf(V3->x * V3->x + V3->y * V3->y + V3->z * V3->z);
	if (rsqrt)
		rsqrt = 1.0f / rsqrt;

	V3->x *= rsqrt; V3->y *= rsqrt; V3->z *= rsqrt;
}
