#include "Primitives/3D/core.hpp"

#include "Math.hpp"
#include "Primitives/2D/core.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <cassert>
#include <cmath>

namespace Primitives3D {

namespace Unit {

void Cilinder()
{
	glPushMatrix();
	{
		glTranslated(0, 0, p);
		Primitives2D::Unit::Circle();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(0, 0, -p);
		Primitives2D::Unit::Circle();
	}
	glPopMatrix();

	const f64 deltaAngle = 0.1;

	// NOTE: Draw the Tube
	glBegin(GL_QUAD_STRIP);
	{
		for (f64 angle = 0; angle < 2 * Math::PId; angle += deltaAngle) {
			const f64 c = cos(angle);
			const f64 s = sin(angle);

			const f64 x = radius * c;
			const f64 y = radius * s;
			glNormal3d(c, s, 0);
			glVertex3d(x, y, p);
			glVertex3d(x, y, -p);
		}
		glVertex3f(radius, 0, p);
		glVertex3f(radius, 0, -p);
	}
	glEnd();
}

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
