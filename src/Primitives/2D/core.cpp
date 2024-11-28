#include "Primitives/2D/core.hpp"

#include "Math.hpp"

#include <GL/gl.h>
#include <cmath>
#include <cstdint>

namespace Primitives2D {

void Unit::Circle(const uint32_t resolution, const bool fill)
{
	using std::cos, std::sin;

	static constexpr GLfloat radius = .5f;
	const GLfloat deltaAngle        = 2 * Math::PId / resolution;

	glBegin(fill ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
	{
		if (fill) {
			glVertex2f(0, 0);
		}

		for (GLfloat angle  = 0; angle < 2 * Math::PId;
		     angle         += deltaAngle) {
			const GLfloat x = radius * cos(angle);
			const GLfloat y = radius * sin(angle);

			glVertex2f(x, y);
		}
	}
	glEnd();
}

void Unit::Square()
{
	static constexpr GLfloat p = .5f;

	glBegin(GL_QUADS);
	{
		glVertex2f(-p, p);
		glVertex2f(-p, -p);
		glVertex2f(p, -p);
		glVertex2f(p, p);
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

} // namespace Primitives2D
