#include "Primitives/2D/core.hpp"

#include "Math.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <cmath>

namespace Primitives2D {

void Unit::Circle()
{
	using std::cos, std::sin, Math::PId;

	static constexpr f64 radius = p;
	const f64 deltaAngle        = 2 * PId / resolution;

	glBegin(GL_POLYGON);
	{
		glNormal3d(0, 0, 1);
		for (f64 angle = 0; angle < 2 * PId; angle += deltaAngle) {
			glVertex2d(radius * cos(angle), radius * sin(angle));
		}
	}
	glEnd();
}

void Unit::Square()
{
	glBegin(GL_QUADS);
	{
		glNormal3d(0, 0, 1);

		glTexCoord2f(0, 0);
		glVertex2f(-p, p);

		glTexCoord2f(0, 1);
		glVertex2f(-p, -p);

		glTexCoord2f(1, 1);
		glVertex2f(p, -p);

		glTexCoord2f(1, 0);
		glVertex2f(p, p);
	}
	glEnd();
}

void Unit::Grid(const u32 rows, const u32 cols)
{
	const f32 cellWidth  = 1.0f / f32(cols);
	const f32 cellHeight = 1.0f / f32(rows);

	glBegin(GL_QUADS);
	{
		glNormal3d(0, 0, 1);
		for (u32 row = 0; row < rows; ++row) {
			for (u32 col = 0; col < cols; ++col) {
				const f32 x1 = -.5f + f32(col) * cellWidth;
				const f32 x2 = x1 + cellWidth;
				const f32 y1 = -.5f + f32(row) * cellHeight;
				const f32 y2 = y1 + cellHeight;

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
