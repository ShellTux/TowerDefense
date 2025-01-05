#include "Primitives/2D/core.hpp"

#include "Math.hpp"
#include "types.hpp"

#ifndef NOOF
	#include "ofMain.h"
#endif

#include <GL/gl.h>
#include <cmath>

#define DRAW_ARRAYS_IMPLEMENTATION

#ifdef DRAW_ARRAYS_IMPLEMENTATION
	#include <array>
	#include <vector>
#endif


namespace Primitives2D {

void Unit::Circle()
{
	using std::cos, std::sin, Math::PId;

	static constexpr f64 radius = p;
	const f64 deltaAngle        = 2 * PId / resolution;

#ifdef DRAW_ARRAYS_IMPLEMENTATION
	static std::vector<f64> vertices{};

	if (vertices.size() == 0) {
		vertices.reserve(resolution * 5);

		for (f64 angle  = 0; angle < 2 * Math::PId;
		     angle     += deltaAngle) {
			const f64 x = radius * cos(angle);
			const f64 y = radius * sin(angle);

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(0);

			const f64 texX = (x / radius) * 0.5 + 0.5;
			const f64 texY = (y / radius) * 0.5 + 0.5;

			vertices.push_back(texX);
			vertices.push_back(texY);
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	{
		const usize size  = 5 * sizeof(vertices[0]);
		const f64 *offset = vertices.data();
		glVertexPointer(3, GL_DOUBLE, size, offset + 0);
		glTexCoordPointer(2, GL_DOUBLE, size, offset + 3);

		glNormal3d(0, 0, 1);
		glDrawArrays(GL_POLYGON, 0, resolution);
	}
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
#else

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
#endif
}

void Unit::Square()
{
#ifdef DRAW_ARRAYS_IMPLEMENTATION
	static constexpr std::array<f64, 32> vertices = {
	    // Position             // Normal          // Texture Coordinates
	    -p, p,  0, 0, 1, 1, 0, 0, // Top left
	    -p, -p, 0, 0, 1, 1, 0, 1, // Bottom left
	    p,  -p, 0, 0, 1, 1, 1, 1, // Bottom right
	    p,  p,  0, 0, 1, 1, 1, 0  // Top right
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	{
		const usize size  = 8 * sizeof(vertices[0]);
		const f64 *offset = vertices.data();

		glVertexPointer(3, GL_DOUBLE, size, offset + 0);
		glNormalPointer(GL_DOUBLE, size, offset + 3);
		glTexCoordPointer(2, GL_DOUBLE, size, offset + 6);

		glDrawArrays(GL_QUADS, 0, 4);
	}
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
#else
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
#endif
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
