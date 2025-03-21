#include "OpenGL/PolygonMode.hpp"

#include "types.hpp"

#ifndef NOOF
	#include "ofMain.h"
#endif

#include <GL/gl.h>
#include <ostream>
#include <sstream>
#include <string>

namespace OpenGL {

PolygonMode::PolygonMode() = default;

PolygonMode &PolygonMode::operator+=(const usize i)
{
	for (usize j = 0; j < i; ++j) {
		switch (mode) {
		case Point: {
			mode = Line;
		} break;
		case Line: {
			mode = Fill;
		} break;
		case Fill: {
			mode = Point;
		} break;
		}
	}

	return *this;
}

std::ostream &operator<<(std::ostream &os, const PolygonMode &polygonMode)
{
	switch (polygonMode.mode) {
	case PolygonMode::Point: {
		os << "Point";
	} break;
	case PolygonMode::Line: {
		os << "Line";
	} break;
	case PolygonMode::Fill: {
		os << "Fill";
	} break;
	}

	return os;
}

std::string PolygonMode::toString() const
{
	std::stringstream string;
	string << *this;
	return string.str();
}

void PolygonMode::apply() const
{
	switch (mode) {
	case Point: {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	} break;
	case Line: {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} break;
	case Fill: {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} break;
	}
}

} // namespace OpenGL
