#include "Primitives/3D/core.hpp"

#include "Math.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#ifndef NOOF
	#include "ofMain.h"
#endif

#include <GL/gl.h>
#include <cassert>
#include <cmath>

/*#define DRAW_ARRAYS_IMPLEMENTATION*/

#ifdef DRAW_ARRAYS_IMPLEMENTATION
	#include <array>
	#include <vector>
#else
	#include "Primitives/2D/core.hpp"
#endif

namespace Primitives3D {

namespace Unit {

void Cylinder()
{
	static constexpr f64 height = p; // Assuming p represents the radius

#ifdef DRAW_ARRAYS_IMPLEMENTATION
	static constexpr i32 numSlices = 100;
	std::vector<f64> vertices;
	std::vector<f64> normals;

	// NOTE: Generate vertices and normals
	for (i32 i = 0; i <= numSlices; ++i) {
		double angle = 2 * Math::PId * i / numSlices;
		double x     = radius * cos(angle);
		double y     = radius * sin(angle);

		// Top face vertex
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(height);
		normals.push_back(0);
		normals.push_back(0);
		normals.push_back(1); // Normal for top face

		// Bottom face vertex
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(-height);
		normals.push_back(0);
		normals.push_back(0);
		normals.push_back(-1); // Normal for bottom face
	}

	// Create the strip for the tube
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY); // Enable normals
	{
		// NOTE: Set the vertex and normal pointers
		glVertexPointer(3, GL_DOUBLE, 0, vertices.data());
		glNormalPointer(GL_DOUBLE, 0, normals.data());

		// Draw the cylinder body using GL_QUAD_STRIP
		for (int i = 0; i < numSlices; ++i) {
			glDrawArrays(GL_QUAD_STRIP, i * 2, 2);
		}
	}
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
#else
	glPushMatrix();
	{
		glTranslated(0, 0, height);
		Primitives2D::Unit::Circle();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(0, 0, -height);
		Primitives2D::Unit::Circle();
	}
	glPopMatrix();

	static constexpr f64 deltaAngle = 0.1;

	glBegin(GL_QUAD_STRIP);
	{
		for (f64 angle  = 0; angle < 2 * Math::PId;
		     angle     += deltaAngle) {
			const f64 c = cos(angle);
			const f64 s = sin(angle);

			const f64 x = radius * c;
			const f64 y = radius * s;
			glNormal3d(c, s, 0);
			glVertex3d(x, y, height);
			glVertex3d(x, y, -height);
		}
		glVertex3f(radius, 0, height);
		glVertex3f(radius, 0, -height);
	}
	glEnd();
#endif
}

void Cube()
{
#ifdef DRAW_ARRAYS_IMPLEMENTATION
	const std::array<f64, 24> vertices = {
	    -p, -p, -p, // 0: back-bottom-left
	    p,  -p, -p, // 1: back-bottom-right
	    p,  p,  -p, // 2: back-top-right
	    -p, p,  -p, // 3: back-top-left
	    -p, -p, p,  // 4: front-bottom-left
	    p,  -p, p,  // 5: front-bottom-right
	    p,  p,  p,  // 6: front-top-right
	    -p, p,  p   // 7: front-top-left
	};

	const std::array<f64, 24> normals = {
	    0,
	    0,
	    -1, // Back face normal
	    0,
	    0,
	    1, // Front face normal
	    -1,
	    0,
	    0, // Left face normal
	    1,
	    0,
	    0, // Right face normal
	    0,
	    1,
	    0, // Top face normal
	    0,
	    -1,
	    0 // Bottom face normal
	};

	const std::array<GLubyte, 24> indices = {
	    0, 1, 2, 3, // back face
	    4, 5, 6, 7, // front face
	    0, 3, 7, 4, // left face
	    1, 2, 6, 5, // right face
	    3, 2, 6, 7, // top face
	    0, 1, 5, 4  // bottom face
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, vertices.data());

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_DOUBLE, 0, normals.data());

	// NOTE: Draw the cube using GL_QUADS
	for (usize i = 0; i < 6; ++i) {
		glDrawElements(GL_QUADS,
		               4,
		               GL_UNSIGNED_BYTE,
		               indices.data() + i * 4);
	}

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
#else
	glPushMatrix();
	{
		for (i32 i = 0; i < 4; ++i) {
			glRotated(90, 0, 1, 0);

			glPushMatrix();
			{
				glTranslated(0, 0, p);
				Primitives2D::Unit::Square();
			}
			glPopMatrix();
		}

		glRotated(90, 1, 0, 0);
		for (i32 i = 0; i < 2; ++i) {
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
