#include "Primitives/3D/core.hpp"

#include <GL/gl.h>
#include <cstdint>

namespace Primitives3D {

inline void Unit::Cube()
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

void Unit::Grid(const uint32_t rows, const uint32_t cols)
{
	const GLfloat cellWidth  = 1.0f / cols;
	const GLfloat cellHeight = 1.0f / rows;

	glBegin(GL_QUADS);
	{
		for (uint32_t r = 0; r < rows; ++r) {
			for (uint32_t c = 0; c < cols; ++c) {
				const GLfloat x1 = -.5f + c * cellWidth;
				const GLfloat x2 = x1 + cellWidth;
				const GLfloat y1 = -.5f + r * cellHeight;
				const GLfloat y2 = y1 + cellHeight;

				glVertex2f(x1, y1);
				glVertex2f(x1, y2);
				glVertex2f(x2, y2);
				glVertex2f(x2, y1);
			}
		}
	}
	glEnd();
}

} // namespace Primitives3D
