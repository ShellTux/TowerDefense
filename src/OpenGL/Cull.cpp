#include "OpenGL/Cull.hpp"

#include "types.hpp"

#ifndef NOOF
	#include "ofMain.h"
#endif

#include <GL/gl.h>
#include <ostream>
#include <sstream>
#include <string>

namespace OpenGL {

Cull::Cull() = default;

Cull &Cull::operator+=(const usize i)
{
	for (usize j = 0; j < i; ++j) {
		switch (culling) {
		case Front: {
			culling = Back;
		} break;
		case Back: {
			culling = FrontBack;
		} break;
		case FrontBack: {
			culling = Disabled;
		} break;
		case Disabled: {
			culling = Front;
		} break;
		}
	}

	return *this;
}


std::ostream &operator<<(std::ostream &os, const Cull &cull)
{
	switch (cull.culling) {
	case Cull::Front: {
		os << "Front";
	} break;
	case Cull::Back: {
		os << "Back";
	} break;
	case Cull::FrontBack: {
		os << "Front And Back";
	} break;
	case Cull::Disabled: {
		os << "Disabled";
	} break;
	}

	return os;
}

std::string Cull::toString() const {
    std::stringstream string;
    string << *this;
    return string.str();
}

void Cull::apply() const
{
	switch (culling) {
	case Front: {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	} break;
	case Back: {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	} break;
	case FrontBack: {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
	} break;
	case Disabled: {
		glDisable(GL_CULL_FACE);
	} break;
	}
}

Cull &Cull::setCullingFace(const enum Culling &cull)
{
	culling = cull;

	return *this;
}

} // namespace OpenGL
