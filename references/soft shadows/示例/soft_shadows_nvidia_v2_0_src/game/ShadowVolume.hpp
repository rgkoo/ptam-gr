
// includes
#include "bleifrei/math/Matrix.hpp"
#include "bleifrei/mesh/Mesh.hpp"
#include "bleifrei/platform/types.hpp"
#include "bleifrei/tex/Texture.hpp"

#include "Light.hpp"
#include "Object.hpp"

#define MAX_EDGES 1000

#pragma pack(push, 4)

struct Wedge {
	bleifrei::math::Vector3 edge[2];
	bleifrei::math::Vector3 top[2];
	bleifrei::math::Vector3 front[2];
	bleifrei::math::Vector3 back[2];
	bleifrei::math::Vector3 svv;
	bleifrei::math::Vector3 middle[4];
};

struct WedgeIndices {
	struct {
		bleifrei::platform::word front[6];
		bleifrei::platform::word left[3];
		bleifrei::platform::word right[3];
		bleifrei::platform::word bottom[12];
	} front_planes;
	struct {
		bleifrei::platform::word back[6];
		bleifrei::platform::word left[3];
		bleifrei::platform::word right[3];
		bleifrei::platform::word bottom[12];
	} back_planes;
	struct {
		bleifrei::platform::word sv_quad[6];
		bleifrei::platform::word top[3];
		bleifrei::platform::word side[9];
	} center_plane;
};

enum LightType {
    SPHERICAL,
    RECTANGULAR,
	TEXTURED
};

class ShadowVolume {
public:
	ShadowVolume(void) : range(100)								{ buildIndices();			}
	~ShadowVolume(void) {}

	void build(Object *object, Light *light);

	void renderUmbraZPass(void);
	void renderUmbraZFail(void);

	unsigned int getEdgeCount(void)								{ return edge_count;		}

	void initWedgeRendering(void);
	void deInitWedgeRendering(void);

	void sendEdgeToFragmentProgram(unsigned int index);
	void renderWedgeBack(unsigned int index);
	void renderWedgeFront(unsigned int index);
	void renderWedgeFrontCW(unsigned int index);
	void renderWedgeFrontCCW(unsigned int index);

private:
	void buildIndices(void);

    void build(float radius);
	void build(const bleifrei::math::Vector3 extents[2], bleifrei::math::Vector3 objspace_normal);

	void buildWedges(LightType type, float radius, const bleifrei::math::Vector3 extents[2], bleifrei::math::Vector3 *objspace_normal = 0);

	void renderWedgeSide(unsigned int uiWedgeIndex, unsigned int uiSection);
	void renderFront(unsigned int index);
	void renderBack(unsigned int index);
	void renderCenterPlane(unsigned int index);

	float range;

	void computeSilhouetteEdges(void);

	bleifrei::math::Vector3	lightcenter;

	bleifrei::platform::dword	edge_count, volume_index_count;

    bleifrei::platform::word	sv_indices[MAX_EDGES * 9];
	Wedge						wedges[MAX_EDGES];
	WedgeIndices				wedge_indices[MAX_EDGES];

	Object *object;
};

#pragma pack(pop)

inline void ShadowVolume::sendEdgeToFragmentProgram(unsigned int index)
{
	glMultiTexCoord3fv(GL_TEXTURE0, &wedges[index].edge[0][0]);
	glMultiTexCoord3fv(GL_TEXTURE1, &wedges[index].edge[1][0]);
}

inline void ShadowVolume::renderWedgeBack(unsigned int index)
{
	renderBack(index);
	renderCenterPlane(index);
}

inline void ShadowVolume::renderWedgeFront(unsigned int index)
{
	renderFront(index);
	renderCenterPlane(index);
}

inline void ShadowVolume::renderWedgeFrontCW(unsigned int index)
{
	renderFront(index);
	glFrontFace(GL_CCW);
	renderCenterPlane(index);
	glFrontFace(GL_CW);
}

inline void ShadowVolume::renderWedgeFrontCCW(unsigned int index)
{
	renderFront(index);
	glFrontFace(GL_CW);
	renderCenterPlane(index);
	glFrontFace(GL_CCW);
}

inline void ShadowVolume::renderFront(unsigned int index)
{
	glDrawElements(GL_TRIANGLES, 8 * 3, GL_UNSIGNED_SHORT, &wedge_indices[index].front_planes.front[0]);
}

inline void ShadowVolume::renderBack(unsigned int index)
{
	glDrawElements(GL_TRIANGLES, 8 * 3, GL_UNSIGNED_SHORT, &wedge_indices[index].back_planes.back[0]);
}

inline void ShadowVolume::renderCenterPlane(unsigned int index)
{
	glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_SHORT, &wedge_indices[index].center_plane.sv_quad[0]);
}
