#include "Primitives/3D/core.hpp"

#include <GL/gl.h>
#include <cstdint>

namespace Primitives3D {

void Unit::Cube()
{
	static constexpr GLfloat p = .5;

	glBegin(GL_QUADS);
	{
		// NOTE: Above
		glVertex3f(-p, -p, -p);
		glVertex3f(-p, -p, p);
		glVertex3f(p, -p, p);
		glVertex3f(p, -p, -p);

		// NOTE: Below
		glVertex3f(-p, p, -p);
		glVertex3f(p, p, -p);
		glVertex3f(p, p, p);
		glVertex3f(-p, p, p);

		// NOTE: Left
		glVertex3f(-p, -p, -p);
		glVertex3f(-p, p, -p);
		glVertex3f(-p, p, p);
		glVertex3f(-p, -p, p);

		// NOTE: Right
		glVertex3f(p, -p, p);
		glVertex3f(p, p, p);
		glVertex3f(p, p, -p);
		glVertex3f(p, -p, -p);

		// NOTE: Behind
		glVertex3f(p, -p, -p);
		glVertex3f(p, p, -p);
		glVertex3f(-p, p, -p);
		glVertex3f(-p, -p, -p);

		// NOTE: Front
		glVertex3f(-p, -p, p);
		glVertex3f(-p, p, p);
		glVertex3f(p, p, p);
		glVertex3f(p, -p, p);
	}
	glEnd();
}

} // namespace Primitives3D
