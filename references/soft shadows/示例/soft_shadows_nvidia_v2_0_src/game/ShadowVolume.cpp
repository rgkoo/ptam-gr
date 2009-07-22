
#include "bleifrei/io/console.hpp"

// includes
#include <fstream>

#include "bleifrei/mesh/Mesh.hpp"
#include "bleifrei/math/Matrix_op.hpp"
#include "bleifrei/math/Plane.hpp"
#include "bleifrei/img/TGAImage.hpp"
#include "bleifrei/platform/types.hpp"

#include "ShadowVolume.hpp"

using namespace bleifrei::math;
using namespace bleifrei::mesh;
using namespace bleifrei::platform;
using namespace bleifrei;
using namespace std;

enum {
	E0 =	0,				// top vertices
    E1 =	1,				//  .
    FP0 =	2,				// front vertices
    FP1 =	3,				//  .
    BP0 =	4,				// back vertices
    BP1 =	5,				//  .
    SVV =	6,				// shadow volume vertex (original edge vertex)
    MP0 =	7,				// middle plane vertices
    MP1 =	8,				//  .
    MP2 =	9,				//  .
    MP3 =	10				//  .
};

void ShadowVolume::buildIndices(void)
{
	platform::word index = 0;
	for (int i = 0; i < MAX_EDGES; ++i) {

		// set indices for front wedge
		wedge_indices[i].front_planes.front[0] = platform::word(index + E0);		// front plane
	    wedge_indices[i].front_planes.front[1] = platform::word(index + E1);
	    wedge_indices[i].front_planes.front[2] = platform::word(index + FP1);
	    wedge_indices[i].front_planes.front[3] = platform::word(index + E0);
	    wedge_indices[i].front_planes.front[4] = platform::word(index + FP1);
	    wedge_indices[i].front_planes.front[5] = platform::word(index + FP0);

	    wedge_indices[i].front_planes.left[0] = platform::word(index + E0);			// left side plane
	    wedge_indices[i].front_planes.left[1] = platform::word(index + FP0);
	    wedge_indices[i].front_planes.left[2] = platform::word(index + MP0);

    	wedge_indices[i].front_planes.right[0] = platform::word(index + E1);		// right side plane
    	wedge_indices[i].front_planes.right[1] = platform::word(index + MP3);
	    wedge_indices[i].front_planes.right[2] = platform::word(index + FP1);
		
    	wedge_indices[i].front_planes.bottom[0] = platform::word(index + MP0);		// bottom plane
    	wedge_indices[i].front_planes.bottom[1] = platform::word(index + FP0);
    	wedge_indices[i].front_planes.bottom[2] = platform::word(index + MP1);

		wedge_indices[i].front_planes.bottom[3] = platform::word(index + FP0);
    	wedge_indices[i].front_planes.bottom[4] = platform::word(index + FP1);
    	wedge_indices[i].front_planes.bottom[5] = platform::word(index + MP1);

		wedge_indices[i].front_planes.bottom[6] = platform::word(index + FP1);
    	wedge_indices[i].front_planes.bottom[7] = platform::word(index + MP2);
    	wedge_indices[i].front_planes.bottom[8] = platform::word(index + MP1);

		wedge_indices[i].front_planes.bottom[9] = platform::word(index + FP1);
    	wedge_indices[i].front_planes.bottom[10] = platform::word(index + MP3);
    	wedge_indices[i].front_planes.bottom[11] = platform::word(index + MP2);

        // set indices for back wedge
	    wedge_indices[i].back_planes.back[0] = platform::word(index + E1);			// back plane
	    wedge_indices[i].back_planes.back[1] = platform::word(index + E0);
	    wedge_indices[i].back_planes.back[2] = platform::word(index + BP0);
	    wedge_indices[i].back_planes.back[3] = platform::word(index + E1);
	    wedge_indices[i].back_planes.back[4] = platform::word(index + BP0);
	    wedge_indices[i].back_planes.back[5] = platform::word(index + BP1);
        
	    wedge_indices[i].back_planes.left[0] = platform::word(index + E0);			//left side plane
	    wedge_indices[i].back_planes.left[1] = platform::word(index + MP0);
	    wedge_indices[i].back_planes.left[2] = platform::word(index + BP0);
        
    	wedge_indices[i].back_planes.right[0] = platform::word(index + E1);			// right side plane
    	wedge_indices[i].back_planes.right[1] = platform::word(index + BP1);
	    wedge_indices[i].back_planes.right[2] = platform::word(index + MP3);
		
    	wedge_indices[i].back_planes.bottom[0] = platform::word(index + MP0);		// bottom plane
    	wedge_indices[i].back_planes.bottom[1] = platform::word(index + MP1);
    	wedge_indices[i].back_planes.bottom[2] = platform::word(index + BP0);

		wedge_indices[i].back_planes.bottom[3] = platform::word(index + BP0);
    	wedge_indices[i].back_planes.bottom[4] = platform::word(index + MP1);
    	wedge_indices[i].back_planes.bottom[5] = platform::word(index + MP2);

		wedge_indices[i].back_planes.bottom[6] = platform::word(index + MP2);
    	wedge_indices[i].back_planes.bottom[7] = platform::word(index + BP1);
    	wedge_indices[i].back_planes.bottom[8] = platform::word(index + BP0);

		wedge_indices[i].back_planes.bottom[9] = platform::word(index + MP2);
    	wedge_indices[i].back_planes.bottom[10] = platform::word(index + MP3);
    	wedge_indices[i].back_planes.bottom[11] = platform::word(index + BP1);

        // set indices for center plane
        wedge_indices[i].center_plane.top[0] = platform::word(index + E0);			// extended edge, sv edge
	    wedge_indices[i].center_plane.top[1] = platform::word(index + E1);
	    wedge_indices[i].center_plane.top[2] = platform::word(index + SVV);

		index += 11 + 2;
	}
}

void ShadowVolume::renderUmbraZPass(void)
{
    if (edge_count == 0) return;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &wedges->top[0][0]);
	glDrawElements(GL_TRIANGLES, edge_count * 6, GL_UNSIGNED_SHORT, sv_indices);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ShadowVolume::renderUmbraZFail(void)
{
	Mesh &m = *object->getMesh();

    if (edge_count == 0) return;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vector3), &m.get_vertices()[0][0]);

	for (unsigned int face = 0; face < m.get_triangles().size(); ++face) {
		glBegin(GL_TRIANGLES);
			if (m.get_planes()[face].distance(lightcenter) > 0.0f) {
				// Near cap
				glArrayElement(m.get_triangles()[face].v[0]);
				glArrayElement(m.get_triangles()[face].v[1]);
				glArrayElement(m.get_triangles()[face].v[2]);
			}
			else {
				// Crude far cap extrusion
				Vector3 v0 = m.get_vertices()[m.get_triangles()[face].v[0]];
				Vector3 v1 = m.get_vertices()[m.get_triangles()[face].v[1]];
				Vector3 v2 = m.get_vertices()[m.get_triangles()[face].v[2]];
				glVertex3fv(&(v0 + normalize(v0 - lightcenter) * range)[0]);
				glVertex3fv(&(v1 + normalize(v1 - lightcenter) * range)[0]);
				glVertex3fv(&(v2 + normalize(v2 - lightcenter) * range)[0]);
			}
		glEnd();
	}

	glVertexPointer(3, GL_FLOAT, 0, &wedges->top[0][0]);

	glDrawElements(GL_TRIANGLES, edge_count * 6, GL_UNSIGNED_SHORT, sv_indices);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ShadowVolume::initWedgeRendering(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &wedges->top[0][0]);
}

void ShadowVolume::deInitWedgeRendering(void)
{
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ShadowVolume::buildWedges(LightType type, float radius, const Vector3 extents[2], Vector3 *objspace_normal)                                             
{
	platform::dword wedge_vertex_count = 0;
	platform::dword sv_index_count = 0;

    for(unsigned int i = 0 ; i < edge_count; ++i) {
        math::Vector3 edge_vertex0 = wedges[i].edge[0];
        math::Vector3 edge_vertex1 = wedges[i].edge[1];

    	math::Vector3 light_to_point0 = edge_vertex0 - lightcenter;					// get rays from light center to edge verts
	    math::Vector3 light_to_point1 = edge_vertex1 - lightcenter;
	    
	    float distance_squared0 = light_to_point0 * light_to_point0;				// compute the squared distance from edge
	    float distance_squared1 = light_to_point1 * light_to_point1;				//  verts to light center

	    // move the farther edge point in along its light->edge ray so that it is the same distance from the light
	    // as the other edge point
	    if (distance_squared0 <= distance_squared1) {
		    light_to_point1 *= sqrtf(distance_squared0 / distance_squared1);
		    wedges[i].top[0] = edge_vertex0;
            wedges[i].top[1] = lightcenter + light_to_point1;
            wedges[i].svv = edge_vertex1;
	    }
	    else {
		    light_to_point0 *= sqrtf(distance_squared1 / distance_squared0);
		    wedges[i].top[0] = lightcenter + light_to_point0;
            wedges[i].top[1] = edge_vertex1;
            wedges[i].svv = edge_vertex0;
	    }
	
		if (type == RECTANGULAR && (*objspace_normal) * (edge_vertex0 - lightcenter) > 0 && (*objspace_normal) * (edge_vertex1 - lightcenter) > 0) {
			wedges[i].front[0] = wedges[i].middle[1];								// If the wedge is above the plane of
			wedges[i].front[1] = wedges[i].middle[2];								//  a rectangular light source we
			wedges[i].back[0] = wedges[i].middle[1];								//  flatten it since there is no need
			wedges[i].back[1] = wedges[i].middle[2];								//  to rasterize it.
			wedges[i].middle[0] = wedges[i].middle[1];
			wedges[i].middle[3] = wedges[i].middle[2];
		}
		else {
			// compute the plane whos x extent axis is E1 - E0 (adjusted) and whos y extent axis is perpendicualr
			// to E1 - E0 (ajusted) x LP0 
			math::Vector3 x_axis, y_axis;
			math::normalize(x_axis, wedges[i].top[1] - wedges[i].top[0]);
			math::cross(y_axis, x_axis, math::operator -(light_to_point1));
			math::normalize(y_axis);

			if (type == RECTANGULAR) {
				x_axis *= fabsf(x_axis * extents[0]) + fabsf(x_axis * extents[1]);	// multiply each extent vector times magnitue
				y_axis *= fabsf(y_axis * extents[0]) + fabsf(y_axis * extents[1]);	//  of the box projected onto each extent
			}
			else {
    			x_axis *= radius;
				y_axis *= radius;
			}

			// Use extents to extrude wedge vertices
			math::Vector3 direction = wedges[i].top[0] - (lightcenter + x_axis + y_axis);	// Extrude front vertex 0
			math::normalize(direction);
			wedges[i].front[0] = wedges[i].top[0] + direction * range;

			direction = wedges[i].top[1] - (lightcenter - x_axis + y_axis);			// Extrude front vertex 1
			math::normalize(direction);
			wedges[i].front[1] = wedges[i].top[1] + direction * range;

			direction = wedges[i].top[0] - (lightcenter + x_axis - y_axis);			// Extrude back vertex 0
			math::normalize(direction);
			wedges[i].back[0] = wedges[i].top[0] + direction * range;

			direction = wedges[i].top[1] - (lightcenter - x_axis - y_axis);			// Extrude back vertex 1
			math::normalize(direction);
			wedges[i].back[1] = wedges[i].top[1] + direction * range;

			direction = wedges[i].top[0] - (lightcenter + x_axis);					// Extrude middle vertex 0
			math::normalize(direction);
			wedges[i].middle[0] = wedges[i].top[0] + direction * range;

			direction = wedges[i].top[1] - (lightcenter - x_axis);					// Extrude middle vertex 3
			math::normalize(direction);
			wedges[i].middle[3] = wedges[i].top[1] + direction * range;

			if (type == SPHERICAL && distance_squared0 > distance_squared1) {		// If it is a spherical light source
				wedges[i].edge[0] = edge_vertex1;									//  make sure that the first edge vertex
				wedges[i].edge[1] = edge_vertex0;									//  is closest to light source center
			}
		}

        // set indices for center plane
        if (distance_squared0 <= distance_squared1) {
	        wedge_indices[i].center_plane.sv_quad[0] = sv_indices[sv_index_count++] = platform::word(wedge_vertex_count + E0);		// shadow volume edge
	        wedge_indices[i].center_plane.sv_quad[1] = sv_indices[sv_index_count++] = platform::word(wedge_vertex_count + SVV);
	        wedge_indices[i].center_plane.sv_quad[2] = sv_indices[sv_index_count++] = platform::word(wedge_vertex_count + MP1);
            wedge_indices[i].center_plane.sv_quad[3] = sv_indices[sv_index_count++] = platform::word(wedge_vertex_count + MP1);
	        wedge_indices[i].center_plane.sv_quad[4] = sv_indices[sv_index_count++] = platform::word(wedge_vertex_count + SVV);
	        wedge_indices[i].center_plane.sv_quad[5] = sv_indices[sv_index_count++] = platform::word(wedge_vertex_count + MP2);
                         
            wedge_indices[i].center_plane.side[0] = platform::word(wedge_vertex_count + MP0);	// left side connecting poly
	        wedge_indices[i].center_plane.side[1] = platform::word(wedge_vertex_count + E0);
	        wedge_indices[i].center_plane.side[2] = platform::word(wedge_vertex_count + MP1);
            
            wedge_indices[i].center_plane.side[3] = platform::word(wedge_vertex_count + MP2);	// right side connecting poly
	        wedge_indices[i].center_plane.side[4] = platform::word(wedge_vertex_count + SVV);
	        wedge_indices[i].center_plane.side[5] = platform::word(wedge_vertex_count + MP3);
            wedge_indices[i].center_plane.side[6] = platform::word(wedge_vertex_count + SVV);
	        wedge_indices[i].center_plane.side[7] = platform::word(wedge_vertex_count + E1);
	        wedge_indices[i].center_plane.side[8] = platform::word(wedge_vertex_count + MP3);
        }
        else {
	        wedge_indices[i].center_plane.sv_quad[0] = sv_indices[sv_index_count++] = platform::word(wedge_vertex_count + SVV);	// shadow volume edge
	        wedge_indices[i].center_plane.sv_quad[1] = sv_indices[sv_index_count++] = platform::word(wedge_vertex_count + E1);
	        wedge_indices[i].center_plane.sv_quad[2] = sv_indices[sv_index_count++] = platform::word(wedge_vertex_count + MP1);
            wedge_indices[i].center_plane.sv_quad[3] = sv_indices[sv_index_count++] = platform::word(wedge_vertex_count + MP1);
	        wedge_indices[i].center_plane.sv_quad[4] = sv_indices[sv_index_count++] = platform::word(wedge_vertex_count + E1);
	        wedge_indices[i].center_plane.sv_quad[5] = sv_indices[sv_index_count++] = platform::word(wedge_vertex_count + MP2);
			
            wedge_indices[i].center_plane.side[0] = platform::word(wedge_vertex_count + MP0);	// left side connecting poly
	        wedge_indices[i].center_plane.side[1] = platform::word(wedge_vertex_count + E0);
	        wedge_indices[i].center_plane.side[2] = platform::word(wedge_vertex_count + SVV);
            wedge_indices[i].center_plane.side[3] = platform::word(wedge_vertex_count + MP0);
	        wedge_indices[i].center_plane.side[4] = platform::word(wedge_vertex_count + SVV);
	        wedge_indices[i].center_plane.side[5] = platform::word(wedge_vertex_count + MP1);
            
            wedge_indices[i].center_plane.side[6] = platform::word(wedge_vertex_count + MP2);	// right side connecting poly
	        wedge_indices[i].center_plane.side[7] = platform::word(wedge_vertex_count + E1);
	        wedge_indices[i].center_plane.side[8] = platform::word(wedge_vertex_count + MP3);
        }
		wedge_vertex_count += 11 + 2;												// 11 vertices and 2 edge coords per wedge
    }
}

void ShadowVolume::build(Object *object, Light *light)
{
	this->object = object;

	lightcenter = *(Vector3 *)&(object->getMatrixInverse() * light->getPosition());	// Transform lightpos into object coords

	if (light->getType() == Light::Spherical) {
		SphericalLight *sphericallight = (SphericalLight *)light;

		build(sphericallight->getRadius());
	}
	else {
		static Vector3 scaledext[2], objspace_normal;

		RectangularLight *rectlight = (RectangularLight *)light;

		assign(objspace_normal, light->getMatrix().getColumn(1));
		objspace_normal = object->getMatrixInverse() * objspace_normal;

		assign(scaledext[0], light->getMatrix().getColumn(0) * rectlight->getWidth());
		assign(scaledext[1], light->getMatrix().getColumn(2) * rectlight->getLength());
		scaledext[0] = object->getMatrixInverse() * scaledext[0];
		scaledext[1] = object->getMatrixInverse() * scaledext[1];

		build(scaledext, objspace_normal);
	}
}

void ShadowVolume::build(float radius)
{
	computeSilhouetteEdges();

	for (unsigned int i = 0; i < edge_count; ++i) {									// Extrude shadow volume
		math::Vector3 vDelta = wedges[i].edge[0] - lightcenter;
		math::normalize(vDelta);
		
		wedges[i].middle[1] = wedges[i].edge[0] + vDelta * range;

		vDelta = wedges[i].edge[1] - lightcenter;
		math::normalize(vDelta);

		wedges[i].middle[2] = wedges[i].edge[1] + vDelta * range;
	}

	buildWedges(SPHERICAL, radius, 0);
}

void ShadowVolume::build(const math::Vector3 extents[2], Vector3 objspace_normal)
{
	computeSilhouetteEdges();

	for (unsigned int i = 0; i < edge_count; ++i) {									// Extrude shadow volume
		math::Vector3 vDelta = wedges[i].edge[0] - lightcenter;						// Vertex 0
		math::normalize(vDelta);
		
		wedges[i].middle[1] = wedges[i].edge[0] + vDelta * range;

		vDelta = wedges[i].edge[1] - lightcenter;
		math::normalize(vDelta);
		
		wedges[i].middle[2] = wedges[i].edge[1] + vDelta * range;
	}

	buildWedges(RECTANGULAR, 0, extents, &objspace_normal);
}

void ShadowVolume::computeSilhouetteEdges(void)
{
	static bool front_facing[MAX_EDGES * 2];

	Mesh *m = object->getMesh();
	const vector<Plane<> > &planes = m->get_planes();
	const vector<Vector3> &vertices = m->get_vertices();

	edge_count = 0;

	for (dword face = 0; face <	planes.size(); face++ )	{							// Determine if the light is in front
		if (planes[face].distance(lightcenter) > 0.0f) {							//  of or behind each face.
			front_facing[face] = true;
		}
		else {
			front_facing[face] = false;
		}
	}
	
	const vector<Edge> &edges = m->get_edges();

	for (dword edge = 0; edge <	edges.size(); edge++) {								// Determine which edges are potential silhouette edges.
		if (front_facing[edges[edge].plane[0]] ^ front_facing[edges[edge].plane[1]]) {
			if (front_facing[edges[edge].plane[1]]) {
				wedges[edge_count].edge[0] = vertices[edges[edge].v[0]];
				wedges[edge_count].edge[1] = vertices[edges[edge].v[1]];
			}
			else {
				wedges[edge_count].edge[0] = vertices[edges[edge].v[1]];
				wedges[edge_count].edge[1] = vertices[edges[edge].v[0]];
			}
			edge_count++;
		}
	}
}
