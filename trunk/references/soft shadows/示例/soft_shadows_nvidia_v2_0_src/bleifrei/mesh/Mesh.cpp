/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include "../gfx/Material.hpp"
#include "../io/console.hpp"
#include "../math/Matrix.hpp"
#include "../math/Matrix_op.hpp"
#include "../io/io.hpp"
#include "../platform/types.hpp"

#include "Mesh.hpp"

using namespace bleifrei::gfx;
using namespace bleifrei::io;
using namespace bleifrei::math;
using namespace bleifrei::platform;
using namespace std;

namespace bleifrei {
	namespace mesh {

		#pragma pack(push, 1)

		struct MeshHeader {
			dword textures;
			dword materials;

			dword vertices;
			dword triangles;
			dword edges;

			byte reserved[8];
		};

		#pragma pack(pop)

		Mesh::Mesh(istream &is, bool calculateEdges) : calculateEdges(calculateEdges) {
			FileId id;												// Read File header
			get(is, id);

			if (!is) {
				console::print(1.0f, 0.0f, 0.0f, "Failed to load mesh!");
				return;
			}

			if (id != string2id("mESH")) {
				console::print(1.0f, 0.0f, 0.0f, "Not a mesh file!");
				return;
			}

			FileVersion ver;
			get(is, ver);

			if (ver.major != 1 || ver.minor != 0) {
				console::print(1.0f, 0.0f, 0.0f, "Unknown mesh version!");
				return;
			}

			MeshHeader header;										// Read mesh file header
			get(is, header);									

			for (dword i = 0; i < header.textures; ++i) {			// Read textures
				char name[256];
				byte length;
				get(is, length);
				is.read(name, (int)length + 1);
				addTexture(name);
			}

			for (dword i = 0; i < header.materials; i++) {			// Read materials
				addMaterial(Material(is));
			}

			for (dword i = 0; i < header.vertices; i++) {			// Read vertices
				Vector3 v;
				get(is, v[0]);
				get(is, v[1]);
				get(is, v[2]);
				addVertex(v);
			}
			
			for (dword i = 0; i < header.vertices; i++) {			// Read normals
				Vector3 n;
				get(is, n[0]);
				get(is, n[1]);
				get(is, n[2]);
				setNormal(i, n);
			}
			
			for (dword i = 0; i < header.vertices; i++) {			// Read texture coordinates
				Vector2 uv;
				get(is, uv[0]);
				get(is, uv[1]);
				addTexCoord(uv);
			}
			
			for (dword i = 0; i < header.triangles; i++) {			// Read triangles
				Triangle tri;
				get(is, tri.v[0]);
				get(is, tri.v[1]);
				get(is, tri.v[2]);
				get(is, tri.texture);
				get(is, tri.material);
				tri.uv[0] = tri.v[0];
				tri.uv[1] = tri.v[1];
				tri.uv[2] = tri.v[2];
				addTriangle(tri);
			}
			
			for (dword i = 0; i < header.edges; i++) {				// Read edges
				Edge e;
				get(is, e.v[0]);
				get(is, e.v[1]);
				get(is, e.plane[0]);
				get(is, e.plane[1]);
				if (!calculateEdges) addEdge(e);
			}
		}

		dword Mesh::addVertex(Vector3 &v) 
		{
			vertices.push_back(v);
			normals.push_back(Vector3());
			return (dword)(vertices.size() - 1);
		}

		dword Mesh::addMaterial(Material &m)
		{
			materials.push_back(m);
			return (dword)(materials.size() - 1);
		}

		dword Mesh::addTexture(const string &name)
		{
			textures.push_back(name);
			return (dword)(textures.size() - 1);
		}
		
		dword Mesh::addTexCoord(Vector2 &uv)
		{
			texcoords.push_back(uv);
			return (dword)(texcoords.size() - 1);
		}

		void Mesh::addTriangle(dword v0, dword v1, dword v2, dword uv0, dword uv1, dword uv2)
		{
			addTriangle(Triangle(v0, v1, v2, uv0, uv1, uv2, material, texture));
		}

		void Mesh::addTriangle(Triangle &tri)
		{
			triangles.push_back(tri);
			planes.push_back(Plane<>(vertices[tri.v[0]], vertices[tri.v[1]], vertices[tri.v[2]]));
			if (calculateEdges) {
				dword plane = (dword)planes.size() - 1;
				addEdge(tri.v[0], tri.v[1], plane);
				addEdge(tri.v[1], tri.v[2], plane);
				addEdge(tri.v[2], tri.v[0], plane);
			}
		}

		void Mesh::setNormal(dword index, Vector3 &normal)
		{
			normals[index] = normal;
		}

		void Mesh::setMaterial(dword materialIndex)
		{
			material = materialIndex;
		}

		void Mesh::setTexture(dword textureIndex)
		{
			texture = textureIndex;
		}

		void Mesh::addEdge(dword v0, dword v1, dword plane)
		{
			list<Edge>::iterator it;
			for (it = unmatched_edges.begin(); it != unmatched_edges.end(); it++) {
				Edge &edge = *it;
				if ((vertices[edge.v[0]] == vertices[v1]) &&
					(vertices[edge.v[1]] == vertices[v0])) {

					edge.plane[1] = plane;
					addEdge(edge);
					unmatched_edges.erase(it);
					return;
				}
			}
			unmatched_edges.push_back(Edge(v0, v1, plane, 0));
		}

		void Mesh::addEdge(Edge &e)
		{
			edges.push_back(e);
		}

		int Mesh::cap(void)
		{
			int capped = 0;

			while (!unmatched_edges.empty()) {
				Edge e0 = unmatched_edges.front();
				unmatched_edges.pop_front();

				capped++;

				bool unmatched = true;
				list<Edge>::iterator it;
				for (it = unmatched_edges.begin(); unmatched && it != unmatched_edges.end(); ++it) {
					if (vertices[e0.v[0]] == vertices[it->v[1]]) {
						Edge e1 = *it;
						unmatched_edges.erase(it);

						planes.push_back(Plane<>(vertices[e0.v[0]], vertices[e1.v[0]], vertices[e0.v[1]]));
						triangles.push_back(Triangle(e0.v[0], e1.v[0], e0.v[1], 0, 0, 0, 0, 0));

						dword plane = (dword)planes.size() - 1;
						addEdge(e1.v[0], e0.v[1], plane);

						e0.plane[1] = e1.plane[1] = plane;
						addEdge(e0);
						addEdge(e1);

						//capped++;
						break;
					}
					else if (vertices[e0.v[1]] == vertices[it->v[0]]) {
						Edge e1 = *it;
						unmatched_edges.erase(it);

						planes.push_back(Plane<>(vertices[e0.v[0]], vertices[e1.v[1]], vertices[e0.v[1]]));
						triangles.push_back(Triangle(e0.v[0], e1.v[1], e0.v[1], 0, 0, 0, 0, 0));

						dword plane = (dword)planes.size() - 1;
						addEdge(e0.v[0], e1.v[1], plane);

						e0.plane[1] = e1.plane[1] = plane;
						addEdge(e0);
						addEdge(e1);

						//capped++;
						break;
					}
				}
			}

			return capped;
		}


		// TODO: kill this
/*		void Mesh::render(void)
		{
			dword material = 0;

			vector<Triangle>::iterator it;
			for (it = triangles.begin(); it != triangles.end(); it++) {
				
				if (it->material != material) materials[material = it->material].bind();
				
				glBegin(GL_TRIANGLES);
				
					for (int i = 0; i < 3; i++) {
						glNormal3fv((float *)&normals[it->v[i]]);
						glVertex3fv((float *)&vertices[it->v[i]]);
					}

				glEnd();

			}	
		}*/

	}
}
