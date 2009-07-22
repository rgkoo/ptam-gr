/*
 *	md2 file utilities
 *	Based on the original work of www.paulsprojects.net and Andre' LaMothe
 */

#include "md2.h"

/*
 *	LoadMD2
 *	Load md2 model data
 */

GLboolean LoadMD2( MD2_MODEL_PTR md2_model, const char *filename )
{
FILE * fp = NULL; //File pointer to model
int flength = -1; //General file lenght
unsigned char *buffer = NULL; //temporary buffer
MD2_HEADER_PTR md2_header; //Pointer to the md2 header
float temp, sx, sy, sz, tx, ty, tz;
int tindex, findex, vindex, cindex, pindex;

	//open the file
	if ( (fp = fopen(filename, "rb")) == NULL )
	{
		printf("<!> Unable to open file %s \n", filename);
		return GL_FALSE;
	}

	//find the lenght of the model file
	fseek(fp, 0, SEEK_END);
	flength = ftell(fp);

	//read in the header
	fseek(fp, 0, SEEK_SET);
	if ( (buffer = (unsigned char *)malloc(flength+1)) == NULL )
	{
		fclose(fp);
		printf("<!> Unable to allocate memory for MD2 model buffer in %s \n", filename);
		return GL_FALSE;
	}

	int bytes_read = fread(buffer, sizeof(unsigned char), flength, fp);
	if ( bytes_read != flength )
	{
		fclose(fp);
		printf("<!> Unable to read MD2 model in %s \n", filename);
		return GL_FALSE;
	}

	//done with the file
	fclose(fp);

	md2_header = (MD2_HEADER_PTR)buffer;

	//check header is correct
	if( (md2_header->identifier != MD2_IDENTIFIER) ||
		(md2_header->version != MD2_VERSION) )
	{
		printf("<!> %s is not a valid md2 file\n", filename);
		return GL_FALSE;
	}

	////////////////////////
	//assign fields to model
	////////////////////////

	//header or default data
	md2_model->num_frames = md2_header->numFrames;
	md2_model->num_polys = md2_header->numPolys;
	md2_model->num_verts = md2_header->numVertices;
	md2_model->num_textcoords = md2_header->numTextcoords;

	md2_model->position.x = 0.0f;
	md2_model->position.y = 24.0f;
	md2_model->position.z = 0.0f;
	md2_model->rotation = -90.0f;
	md2_model->color.x = 1.0f;
	md2_model->color.y = 1.0f;
	md2_model->color.z = 1.0f;

	md2_model->skin   = (IMAGE_PTR)malloc(sizeof(IMAGE));
	md2_model->normal = (IMAGE_PTR)malloc(sizeof(IMAGE));
	md2_model->skin->data   = NULL;
	md2_model->normal->data = NULL;

	md2_model->anim_state = ANIMATION_STANDING_IDLE;
	md2_model->sec_per_frame = .25f;
	md2_model->anim_time = 0.0f;
	md2_model->anim_counter = 0;
	md2_model->anim_jump = 1;
	md2_model->anim_command = ANIMATION_PAUSE;
	md2_model->current_animation = ANIMATION_STANDING_IDLE;
	md2_model->change_animation = GL_FALSE;

	md2_model->delta_rate = 0.f;
	md2_model->delta_x = 0.f;
	md2_model->delta_z = 0.f;

	//polygon list
	if ( (md2_model->polys = (MD2_POLY_PTR)malloc(md2_header->numPolys*sizeof(MD2_POLY))) == NULL)
	{
		if(buffer)
			free(buffer);
		printf("<!> Unable to allocate memory for %d MD2 model polygons in %s\n", 
			md2_header->numPolys, filename);
		return GL_FALSE;
	}

	//vertex list
	if ( (md2_model->vlist = (VECTOR3D_PTR)malloc(md2_header->numFrames*md2_header->numVertices*sizeof(VECTOR3D))) == NULL )
	{
		if(buffer)
			free(buffer);
		printf("<!> Unable to allocate memory for %d MD2 model vertices in %s\n",
			md2_header->numFrames*md2_header->numVertices, filename);
		return GL_FALSE;
	}

	//TBN lists
	if ( (md2_model->normlist = (VECTOR3D_PTR)malloc(md2_header->numFrames*md2_header->numVertices*sizeof(VECTOR3D))) == NULL)
	{
		if(buffer)
			free(buffer);
		printf("<!> Unable to allocate memory for %d MD2 model polygon normals in %s\n",
			md2_header->numFrames*md2_header->numVertices, filename);
		return GL_FALSE;
	}

	if ( (md2_model->tanglist = (VECTOR3D_PTR)calloc(md2_header->numFrames*md2_header->numVertices, sizeof(VECTOR3D))) == NULL)
	{
		if(buffer)
			free(buffer);
		printf("<!> Unable to allocate memory for %d MD2 model polygon tangents in %s\n",
			md2_header->numFrames*md2_header->numVertices, filename);
		return GL_FALSE;
	}

	if ( (md2_model->binolist = (VECTOR3D_PTR)calloc(md2_header->numFrames*md2_header->numVertices, sizeof(VECTOR3D))) == NULL)
	{
		if(buffer)
			free(buffer);
		printf("<!> Unable to allocate memory for %d MD2 model polygon bi-normals in %s\n",
			md2_header->numFrames*md2_header->numVertices, filename);
		return GL_FALSE;
	}

	//texture coordinates list
	if ( (md2_model->tlist = (VECTOR2D_PTR)malloc(md2_header->numTextcoords*sizeof(VECTOR2D))) == NULL )
	{
		if(buffer)
			free(buffer);
		printf("<!> Unable to allocate memory for %d MD2 model texture coordinates in %s\n",
			md2_header->numTextcoords, filename);
		return GL_FALSE;
	}

	//openGL commands list
	if ( (md2_model->GL_commands = (int *)malloc(md2_header->numGLCommands*sizeof(int))) == NULL )
	{
		if(buffer)
			free(buffer);
		printf("<!> Unable to allocate memory for %d MD2 model OpenGL commands in %s\n",
			md2_header->numGLCommands, filename);
		return GL_FALSE;
	}

	//fill texture coordinates list
	for ( tindex = 0; tindex < md2_header->numTextcoords; ++tindex )
	{
		md2_model->tlist[tindex].x = (float)((MD2_TEXTCOORD_PTR)(buffer+md2_header->offsetTextcoords))[tindex].u / (float) md2_header->skinWidth;
		md2_model->tlist[tindex].y = (float)((MD2_TEXTCOORD_PTR)(buffer+md2_header->offsetTextcoords))[tindex].v / (float) md2_header->skinHeight;
	}

	//fill vertex and normal lists
	for ( findex = 0; findex < md2_header->numFrames; ++findex )
	{
		MD2_FRAME_PTR frame_ptr = (MD2_FRAME_PTR)(buffer
				+ md2_header->offsetFrames
				+ md2_header->frameSize * findex );

		// extract md2 scale and translate
		sx = frame_ptr->scale[0],
		sy = frame_ptr->scale[1],
		sz = frame_ptr->scale[2],
		tx = frame_ptr->translate[0],
		ty = frame_ptr->translate[1],
		tz = frame_ptr->translate[2];

		for ( vindex = 0; vindex < md2_header->numVertices; ++vindex )
		{
			//decompress (scale and translate) compressed vertices and insert in vertex list
			// md2 models assume y == z!
			md2_model->vlist[vindex + (findex * md2_model->num_verts)].x =  (float)frame_ptr->vlist[vindex].v[0] * sx + tx;
			md2_model->vlist[vindex + (findex * md2_model->num_verts)].z = -(float)frame_ptr->vlist[vindex].v[1] * sy - ty;
			md2_model->vlist[vindex + (findex * md2_model->num_verts)].y =  (float)frame_ptr->vlist[vindex].v[2] * sz + tz;

			md2_model->normlist[vindex + (findex * md2_model->num_verts)].x = NORMALS[(GLuint)frame_ptr->vlist[vindex].normal_index*3];
			md2_model->normlist[vindex + (findex * md2_model->num_verts)].y = NORMALS[(GLuint)frame_ptr->vlist[vindex].normal_index*3+2];
			md2_model->normlist[vindex + (findex * md2_model->num_verts)].z = -NORMALS[(GLuint)frame_ptr->vlist[vindex].normal_index*3+1];
		}
	}

	//fill OpenGL command list
	for ( cindex = 0; cindex < md2_header->numGLCommands; ++cindex )
		md2_model->GL_commands[cindex] = ((int *)(buffer+md2_header->offsetGLCommands))[cindex];
	
	//fill polygon list
	MD2_POLY_PTR poly_ptr = (MD2_POLY_PTR)(buffer + md2_header->offsetPolys);

	for ( pindex = 0; pindex < md2_header->numPolys; ++pindex )
	{
		//vertex indices
		md2_model->polys[pindex].vindex[0] = poly_ptr[pindex].vindex[0];
		md2_model->polys[pindex].vindex[1] = poly_ptr[pindex].vindex[1];
		md2_model->polys[pindex].vindex[2] = poly_ptr[pindex].vindex[2];

		//texture coordinate indices
		md2_model->polys[pindex].tindex[0] = poly_ptr[pindex].tindex[0];
		md2_model->polys[pindex].tindex[1] = poly_ptr[pindex].tindex[1];
		md2_model->polys[pindex].tindex[2] = poly_ptr[pindex].tindex[2];
	}

	//Prepare tangent and binormal list, for each poligon and for each frame of animation
	for ( findex = 0; findex < md2_header->numFrames; ++findex )
	for ( pindex = 0; pindex < md2_header->numPolys; ++pindex )
	{
	VECTOR3D SIDE1, SIDE2;
	VECTOR2D DT1, DT2;
	GLuint index;

		index = pindex + findex * md2_header->numVertices;

		//compute the two planar vectors - md2 models are spun counter-clockwise!
		sub(	&md2_model->vlist[md2_model->polys[pindex].vindex[1]+findex * md2_header->numVertices],
			&md2_model->vlist[md2_model->polys[pindex].vindex[0]+findex * md2_header->numVertices],
			&SIDE1 );

		sub(	&md2_model->vlist[md2_model->polys[pindex].vindex[2]+findex * md2_header->numVertices],
			&md2_model->vlist[md2_model->polys[pindex].vindex[0]+findex * md2_header->numVertices],
			&SIDE2 );

		DT1.u = md2_model->tlist[md2_model->polys[pindex].tindex[1]].u - md2_model->tlist[md2_model->polys[pindex].tindex[0]].u;
		DT1.v = md2_model->tlist[md2_model->polys[pindex].tindex[1]].v - md2_model->tlist[md2_model->polys[pindex].tindex[0]].v;
		
		DT2.u = md2_model->tlist[md2_model->polys[pindex].tindex[2]].u - md2_model->tlist[md2_model->polys[pindex].tindex[0]].u;
		DT2.v = md2_model->tlist[md2_model->polys[pindex].tindex[2]].v - md2_model->tlist[md2_model->polys[pindex].tindex[0]].v;

		//Check for malformed models
		if ( ( temp = (DT1.u * DT2.v - DT2.u * DT1.v) ) == 0.f)
			temp = 1.f;

		//compute the tangent vector
		md2_model->tanglist[md2_model->polys[pindex].vindex[0]+findex * md2_header->numVertices].x +=
			( SIDE1.x * DT2.v - SIDE2.x * DT1.v ) / temp;
		md2_model->tanglist[md2_model->polys[pindex].vindex[1]+findex * md2_header->numVertices].x +=
			( SIDE1.x * DT2.v - SIDE2.x * DT1.v ) / temp;
		md2_model->tanglist[md2_model->polys[pindex].vindex[2]+findex * md2_header->numVertices].x +=
			( SIDE1.x * DT2.v - SIDE2.x * DT1.v ) / temp;

		md2_model->tanglist[md2_model->polys[pindex].vindex[0]+findex * md2_header->numVertices].y +=
			( SIDE1.y * DT2.v - SIDE2.y * DT1.v ) / temp;
		md2_model->tanglist[md2_model->polys[pindex].vindex[1]+findex * md2_header->numVertices].y +=
			( SIDE1.y * DT2.v - SIDE2.y * DT1.v ) / temp;
		md2_model->tanglist[md2_model->polys[pindex].vindex[2]+findex * md2_header->numVertices].y +=
			( SIDE1.y * DT2.v - SIDE2.y * DT1.v ) / temp;

		md2_model->tanglist[md2_model->polys[pindex].vindex[0]+findex * md2_header->numVertices].z +=
			( SIDE1.z * DT2.v - SIDE2.z * DT1.v ) / temp;
		md2_model->tanglist[md2_model->polys[pindex].vindex[1]+findex * md2_header->numVertices].z +=
			( SIDE1.z * DT2.v - SIDE2.z * DT1.v ) / temp;
		md2_model->tanglist[md2_model->polys[pindex].vindex[2]+findex * md2_header->numVertices].z +=
			( SIDE1.z * DT2.v - SIDE2.z * DT1.v ) / temp;

		//compute the binormal vector
		md2_model->binolist[md2_model->polys[pindex].vindex[0]+findex * md2_header->numVertices].x +=
			( SIDE2.x * DT1.u - SIDE1.x * DT2.u ) / temp;
		md2_model->binolist[md2_model->polys[pindex].vindex[1]+findex * md2_header->numVertices].x +=
			( SIDE2.x * DT1.u - SIDE1.x * DT2.u ) / temp;
		md2_model->binolist[md2_model->polys[pindex].vindex[2]+findex * md2_header->numVertices].x +=
			( SIDE2.x * DT1.u - SIDE1.x * DT2.u ) / temp;

		md2_model->binolist[md2_model->polys[pindex].vindex[0]+findex * md2_header->numVertices].y +=
			( SIDE2.y * DT1.u - SIDE1.y * DT2.u ) / temp;
		md2_model->binolist[md2_model->polys[pindex].vindex[1]+findex * md2_header->numVertices].y +=
			( SIDE2.y * DT1.u - SIDE1.y * DT2.u ) / temp;
		md2_model->binolist[md2_model->polys[pindex].vindex[2]+findex * md2_header->numVertices].y +=
			( SIDE2.y * DT1.u - SIDE1.y * DT2.u ) / temp;

		md2_model->binolist[md2_model->polys[pindex].vindex[0]+findex * md2_header->numVertices].z +=
			( SIDE2.z * DT1.u - SIDE1.z * DT2.u ) / temp;
		md2_model->binolist[md2_model->polys[pindex].vindex[1]+findex * md2_header->numVertices].z +=
			( SIDE2.z * DT1.u - SIDE1.z * DT2.u ) / temp;
		md2_model->binolist[md2_model->polys[pindex].vindex[2]+findex * md2_header->numVertices].z +=
			( SIDE2.z * DT1.u - SIDE1.z * DT2.u ) / temp;
	}

	vindex = 0;

	// Orthogonalise and compute Binormal list
	do
	{
	GLfloat DotNT;
	VECTOR3D tmp1, tmp2;

		tmp1.x = md2_model->tanglist[vindex].x;
		tmp1.y = md2_model->tanglist[vindex].y;
		tmp1.z = md2_model->tanglist[vindex].z;

		// Gram-Schmidt orthogonalise
		DotNT = scalar_product( &md2_model->normlist[vindex], &tmp1 );

		md2_model->tanglist[vindex].x -= md2_model->normlist[vindex].x * DotNT;
		md2_model->tanglist[vindex].y -= md2_model->normlist[vindex].y * DotNT;
		md2_model->tanglist[vindex].z -= md2_model->normlist[vindex].z * DotNT;

		normalise( &md2_model->tanglist[vindex] );

		// Compute Binormal
		cross_product ( &md2_model->normlist[vindex],
				&tmp1,
				&tmp2 );

		tmp1.x = md2_model->binolist[vindex].x;
		tmp1.y = md2_model->binolist[vindex].y;
		tmp1.z = md2_model->binolist[vindex].z;

		cross_product_norm( &md2_model->normlist[vindex],
				&md2_model->tanglist[vindex],
				&md2_model->binolist[vindex] );

		if( scalar_product( &tmp1, &tmp2 ) < 0.0f )
		{
			md2_model->binolist[vindex].x = - md2_model->binolist[vindex].x;
			md2_model->binolist[vindex].y = - md2_model->binolist[vindex].y;
			md2_model->binolist[vindex].z = - md2_model->binolist[vindex].z;
		}
	}
	while (++vindex < md2_header->numFrames*md2_header->numVertices);

	if(buffer)
		free(buffer);

	return GL_TRUE;
}

/*
 * UpdateMD2
 * Update the md2 model kinematic
 */
void UpdateMD2(MD2_MODEL_PTR md2_model, GLdouble dt )
{
	if (md2_model->current_animation == ANIMATION_RUN)
	{
		md2_model->rotation += md2_model->delta_rate*dt;
		if (md2_model->rotation > 360.f )
			md2_model->rotation -= 360.f;

		md2_model->position.x += ( md2_model->delta_x + md2_model->delta_rate * deg2rad * md2_model->position.z ) * dt;
		md2_model->position.z += ( md2_model->delta_z - md2_model->delta_rate * deg2rad * md2_model->position.x ) * dt;
	}
}

/*
 * AnimateMD2
 * Animate the md2 model
 */
void AnimateMD2( MD2_MODEL_PTR md2_model, GLdouble dt )
{
	switch(md2_model->anim_command)
	{
	case ANIMATION_START:
		md2_model->anim_time = 0.0f;
		md2_model->anim_counter = anim_begin[md2_model->anim_state];
		md2_model->anim_jump = 1;
		md2_model->anim_command = ANIMATION_CONTINUE;
		md2_model->current_animation = md2_model->anim_state;
		break;
	case ANIMATION_PAUSE:
		break;
	case ANIMATION_CHANGE:
		md2_model->anim_time += dt;
		if (md2_model->anim_time >= md2_model->sec_per_frame)
		{
			md2_model->anim_time = 0.0f;
			md2_model->anim_counter += 1;
			if ( md2_model->anim_counter <= anim_end[md2_model->current_animation] && !md2_model->change_animation)
			{
				md2_model->anim_jump = anim_begin[md2_model->anim_state]-md2_model->anim_counter;
				md2_model->change_animation = GL_TRUE;
			}
			else
			{
				md2_model->anim_counter = anim_begin[md2_model->anim_state];
				md2_model->anim_jump = 1;
				md2_model->anim_command = ANIMATION_CONTINUE;
				md2_model->current_animation = md2_model->anim_state;
				md2_model->change_animation = GL_FALSE;
			}
		}
		break;
	case ANIMATION_CONTINUE:
		md2_model->anim_time += dt;
		if (md2_model->anim_time >= md2_model->sec_per_frame)
		{
		md2_model->anim_time = 0.0f;
		md2_model->anim_jump = 1;
		md2_model->anim_counter += 1;
		if ( md2_model->anim_counter == anim_end[md2_model->current_animation] )
		{
			if (md2_model->current_animation == ANIMATION_DEATH_SLOW)
				md2_model->anim_command = ANIMATION_PAUSE;
			md2_model->anim_jump = anim_begin[md2_model->anim_state]-md2_model->anim_counter;
		}
		else if (md2_model->anim_counter > anim_end[md2_model->current_animation])
					md2_model->anim_counter = anim_begin[md2_model->anim_state];
		}
		break;
	}
}

/*
 * DrawMD2
 * Draw the md2 model
 */
void DrawMD2( MD2_MODEL_PTR md2_model )
{
int *ptr_commands = md2_model->GL_commands;	// pointer to gl commands
int i;
GLfloat ratio;

    ratio = (float)(md2_model->anim_time / md2_model->sec_per_frame);

    // draw each triangle!
    while( i = *ptr_commands++ )
    {
        if( i < 0 )
        {
            glBegin( GL_TRIANGLE_FAN );
            i = -i;
        }
        else
            glBegin( GL_TRIANGLE_STRIP );

        for( ; i > 0; --i, ptr_commands += 3 )
        {
	VECTOR3D temp;
	GLuint index_old, index_new;

		index_old = ptr_commands[2] + md2_model->anim_counter * md2_model->num_verts;
		index_new = index_old + md2_model->anim_jump * md2_model->num_verts;

		// parse texture coordinates
		glTexCoord2f( ((float *)ptr_commands)[0], ((float *)ptr_commands)[1]);

		// interpolate tangent vector
		md2_model->old_frameT.x = md2_model->tanglist[index_old].x;
		md2_model->old_frameT.y = md2_model->tanglist[index_old].y;
		md2_model->old_frameT.z = md2_model->tanglist[index_old].z;
			
		md2_model->new_frameT.x = md2_model->tanglist[index_new].x;
		md2_model->new_frameT.y = md2_model->tanglist[index_new].y;
		md2_model->new_frameT.z = md2_model->tanglist[index_new].z;

		temp.x = md2_model->old_frameT.x * (1.0f-ratio) +
			 md2_model->new_frameT.x * ratio;
		temp.y = md2_model->old_frameT.y * (1.0f-ratio) +
			 md2_model->new_frameT.y * ratio;
		temp.z = md2_model->old_frameT.z * (1.0f-ratio) +
			 md2_model->new_frameT.z * ratio;

		glVertexAttrib3fv( md2_model->tangent, temp.M);

		// interpolate bi-normal vector
		md2_model->old_frameB.x = md2_model->binolist[index_old].x;
		md2_model->old_frameB.y = md2_model->binolist[index_old].y;
		md2_model->old_frameB.z = md2_model->binolist[index_old].z;
			
		md2_model->new_frameB.x = md2_model->binolist[index_new].x;
		md2_model->new_frameB.y = md2_model->binolist[index_new].y;
		md2_model->new_frameB.z = md2_model->binolist[index_new].z;

		temp.x = md2_model->old_frameB.x * (1.0f-ratio) +
			 md2_model->new_frameB.x * ratio;
		temp.y = md2_model->old_frameB.y * (1.0f-ratio) +
			 md2_model->new_frameB.y * ratio;
		temp.z = md2_model->old_frameB.z * (1.0f-ratio) +
			 md2_model->new_frameB.z * ratio;

		glVertexAttrib3fv( md2_model->binormal, temp.M);

		// interpolate the normal vector
		md2_model->old_frameN.x = md2_model->normlist[index_old].x;
		md2_model->old_frameN.y = md2_model->normlist[index_old].y;
		md2_model->old_frameN.z = md2_model->normlist[index_old].z;
			
		md2_model->new_frameN.x = md2_model->normlist[index_new].x;
		md2_model->new_frameN.y = md2_model->normlist[index_new].y;
		md2_model->new_frameN.z = md2_model->normlist[index_new].z;

		glNormal3f( 	md2_model->old_frameN.x * (1.0f-ratio) +
				md2_model->new_frameN.x * ratio,
				md2_model->old_frameN.y * (1.0f-ratio) +
				md2_model->new_frameN.y * ratio,
				md2_model->old_frameN.z * (1.0f-ratio) +
				md2_model->new_frameN.z * ratio );

		// interpolate the vertex
		md2_model->old_frameV.x = md2_model->vlist[index_old].x;
		md2_model->old_frameV.y = md2_model->vlist[index_old].y;
		md2_model->old_frameV.z = md2_model->vlist[index_old].z;
			
		md2_model->new_frameV.x = md2_model->vlist[index_new].x;
		md2_model->new_frameV.y = md2_model->vlist[index_new].y;
		md2_model->new_frameV.z = md2_model->vlist[index_new].z;

		glVertex3f( 	md2_model->old_frameV.x * (1.0f-ratio) +
				md2_model->new_frameV.x * ratio,
				md2_model->old_frameV.y * (1.0f-ratio) +
				md2_model->new_frameV.y * ratio,
				md2_model->old_frameV.z * (1.0f-ratio) +
				md2_model->new_frameV.z * ratio );
        }
        glEnd();
    }
}
