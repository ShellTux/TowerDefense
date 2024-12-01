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
			const f64 x = radius * cos(angle);
			const f64 y = radius * sin(angle);

			const f64 texCoordX = (x / radius) * 0.5 + 0.5;
			const f64 texCoordY = (y / radius) * 0.5 + 0.5;

			glTexCoord2d(texCoordX, texCoordY);
			glVertex2d(x, y);
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
	const f64 cellWidth  = 1. / f64(cols);
	const f64 cellHeight = 1. / f64(rows);

	for (u32 row = 0; row < rows; ++row) {
		for (u32 col = 0; col < cols; ++col) {
			const f64 x = -.5 + cellWidth * (f64(col) + .5);
			const f64 y = .5 - cellHeight * (f64(row) + .5);

			glPushMatrix();
			{
				glTranslated(x, y, 0);
				glScaled(cellWidth, cellHeight, 1);
				Square();
			}
			glPopMatrix();
		}
	}
}

} // namespace Primitives2D
