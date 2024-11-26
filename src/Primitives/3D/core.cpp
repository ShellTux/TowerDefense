#include "Primitives/3D/core.hpp"

#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <cassert>

namespace Primitives3D {

namespace Unit {

void Cube()
{
	static constexpr f32 p = .5;

	glBegin(GL_QUADS);
	{
		// NOTE: Above
		glNormal3d(0, 1, 0);
		glVertex3f(-p, -p, -p);
		glVertex3f(-p, -p, p);
		glVertex3f(p, -p, p);
		glVertex3f(p, -p, -p);

		// NOTE: Below
		glNormal3d(0, -1, 0);
		glVertex3f(-p, p, -p);
		glVertex3f(p, p, -p);
		glVertex3f(p, p, p);
		glVertex3f(-p, p, p);

		// NOTE: Left
		glNormal3d(-1, 0, 0);
		glVertex3f(-p, -p, -p);
		glVertex3f(-p, p, -p);
		glVertex3f(-p, p, p);
		glVertex3f(-p, -p, p);

		// NOTE: Right
		glNormal3d(1, 0, 0);
		glVertex3f(p, -p, p);
		glVertex3f(p, p, p);
		glVertex3f(p, p, -p);
		glVertex3f(p, -p, -p);

		// NOTE: Behind
		glNormal3d(0, 0, -1);
		glVertex3f(p, -p, -p);
		glVertex3f(p, p, -p);
		glVertex3f(-p, p, -p);
		glVertex3f(-p, -p, -p);

		// NOTE: Front
		glNormal3d(0, 0, 1);
		glVertex3f(-p, -p, p);
		glVertex3f(-p, p, p);
		glVertex3f(p, p, p);
		glVertex3f(p, -p, p);
	}
	glEnd();
}

} // namespace Unit

void Cube(const f32 x, const f32 y, const f32 z)
{
	(void) x;
	(void) y;
	(void) z;

	assert(false && "Not yet Implemented\n");
}

void Cube(const Vec3 &pos)
{
	(void) pos;

	assert(false && "Not yet Implemented\n");
}

} // namespace Primitives3D
