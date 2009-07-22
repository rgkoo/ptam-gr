
// includes
#include "bleifrei/gfx/opengl.hpp"

#include "PlayerCamera.hpp"

using namespace bleifrei::math;
using namespace bleifrei;

const float distance = -0.5f;

void PlayerCamera::update(void)
{
	float direction = player.getDirection();
	float pitch = player.getPitch();
	const Vector3 &target = player.getEyePosition();

	glLoadIdentity();
	gluLookAt(	target[0] + distance * cos(direction) * cos(pitch), target[1] + distance * sin(pitch), target[2] + distance * sin(direction) * cos(pitch),
				target[0], target[1], target[2],
				0,1,0);
	glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0]);
}
