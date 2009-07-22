

// includes
#include "Ground.hpp"

using namespace bleifrei::mesh;
using namespace bleifrei::math;
using namespace std;

Ground::Ground(dSpaceID space, bleifrei::mesh::Mesh *mesh, bleifrei::mesh::Model model)
	: ODEStaticObject(mesh, model)
{
	const vector<Vector3> &vertices = mesh->get_vertices();
	const vector<Triangle> &triangles = mesh->get_triangles();

	tmData = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuild(tmData, &vertices[0], sizeof(Vector3), (int)vertices.size(),
						&triangles[0], (int)triangles.size() * 3, sizeof Triangle);

	geometry = dCreateTriMesh(space, tmData, 0, 0, 0);
}

Ground::~Ground(void)
{
	dGeomTriMeshDataDestroy(tmData);
}