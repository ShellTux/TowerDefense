#include "Primitives/3D/core.hpp"

#include "Primitives/2D/core.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <cassert>

namespace Primitives3D {

namespace Unit {

void Cube()
{
#if PRIMITIVES2D_USE
	glPushMatrix();
	{
		for (int i = 0; i < 4; ++i) {
			glRotated(90, 0, 1, 0);

			glPushMatrix();
			{
				glTranslated(0, 0, p);
				Primitives2D::Unit::Square();
			}
			glPopMatrix();
		}

		glRotated(90, 1, 0, 0);
		for (int i = 0; i < 2; ++i) {
			glRotated(180, 1, 0, 0);

			glPushMatrix();
			{
				glTranslated(0, 0, p);
				Primitives2D::Unit::Square();
			}
			glPopMatrix();
		}
	}
	glPopMatrix();
#else
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
#endif
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
