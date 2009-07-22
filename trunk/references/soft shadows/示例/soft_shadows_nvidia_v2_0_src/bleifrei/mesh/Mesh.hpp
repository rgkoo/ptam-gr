/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_Mesh_hpp_
#define _include_bleifrei_Mesh_hpp_


// includes
#include <list>
#include <vector>

#include "../gfx/Material.hpp"
#include "../math/Matrix.hpp"
#include "../math/Plane.hpp"
#include "../platform/types.hpp"

namespace bleifrei {
	namespace mesh {

		class Triangle {
		public:
			Triangle(void) {}
			Triangle(platform::dword v0, platform::dword v1, platform::dword v2,
				platform::dword uv0, platform::dword uv1, platform::dword uv2,
				platform::dword material, platform::dword texture)
			{
				v[0] = v0;
				v[1] = v1;
				v[2] = v2;
				uv[0] = uv0;
				uv[1] = uv1;
				uv[2] = uv2;
				this->material = material;
				this->texture = texture;
			}
			platform::dword v[3];
			platform::dword uv[3];
			platform::dword material;
			platform::dword texture;
		};
	
		class Edge {
		public:
			Edge(void) {}
			Edge(platform::dword v0, platform::dword v1,
				platform::dword plane0, platform::dword plane1)
			{
				v[0] = v0;
				v[1] = v1;
				this->plane[0] = plane0;
				this->plane[1] = plane1;
			}
			platform::dword v[2];
			platform::dword plane[2];
		};



		class Mesh {
		public:
			Mesh(std::istream &is, bool calculateEdges = false);
			platform::dword addVertex(math::Vector3 &v);
			platform::dword addTexCoord(math::Vector2 &uv);
			platform::dword addMaterial(gfx::Material &m);
			platform::dword addTexture(const std::string &name);

			void setNormal(platform::dword index, math::Vector3 &normal);
			void addEdge(platform::dword v0, platform::dword v1, platform::dword plane);
			void addEdge(Edge &e);
			void addTriangle(platform::dword v0, platform::dword v1, platform::dword v2, platform::dword uv0, platform::dword uv1, platform::dword uv2);
			void addTriangle(Triangle &tri);

			void setMaterial(platform::dword materialIndex);
			void setTexture(platform::dword textureIndex);

			int cap(void);

			std::vector<math::Vector3>	&get_vertices(void)			{ return vertices;			}
			std::vector<math::Vector3>	&get_normals(void)			{ return normals;			}
			std::vector<math::Vector2>	&get_texcoords(void)		{ return texcoords;			}
			std::vector<math::Plane<> >	&get_planes(void)			{ return planes;			}
			std::vector<gfx::Material>	&get_materials(void)		{ return materials;			}
			std::vector<std::string>	&get_textures(void)			{ return textures;			}
			std::vector<Triangle>		&get_triangles(void)		{ return triangles;			}
			std::vector<Edge>			&get_edges(void)			{ return edges;				}
			std::list<Edge>				&get_unmatched_edges(void)	{ return unmatched_edges;	}

			// ugh!
			void render(void);

		private:
			bool calculateEdges;

			platform::dword material;										// Holds default material index
			platform::dword texture;										// Holds default texture index

			std::vector<math::Vector3> vertices;
			std::vector<math::Vector3> normals;
			std::vector<math::Vector2> texcoords;
			std::vector<math::Plane<> > planes;
			std::vector<gfx::Material> materials;
			std::vector<std::string> textures;
			std::vector<Triangle> triangles;
			std::vector<Edge> edges;
			std::list<Edge> unmatched_edges;
		};

	}
}

#endif //_include_bleifrei_Mesh_hpp_
