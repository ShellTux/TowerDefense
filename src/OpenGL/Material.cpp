#include "OpenGL/Material.hpp"

#include <GL/gl.h>
#include <stdexcept>

namespace OpenGL {

struct Material::Specs Material::getSpecs(const Material::Type &type)
{
	switch (type) {
	case BlackPlastic:
		return {
		    { 0.0f,  0.0f,  0.0f, 1.0f},
		    {0.01f, 0.01f, 0.01f, 1.0f},
		    {0.50f, 0.50f, 0.50f, 1.0f},
		    32
                };

	case Brass:
		return {
		    {0.25f, 0.22f, 0.16f, 1.0f},
		    {0.78f, 0.57f, 0.11f, 1.0f},
		    {0.99f, 0.91f, 0.81f, 1.0f},
		    27
                };

	case Bronze:
		return {
		    {0.33f, 0.22f, 0.03f, 1.0f},
		    {0.78f, 0.57f, 0.11f, 1.0f},
		    {0.99f, 0.91f, 0.81f, 1.0f},
		    30
                };

	case Chrome:
		return {
		    {0.25f, 0.25f, 0.25f, 1.0f},
		    { 0.4f,  0.4f,  0.4f, 1.0f},
		    {0.77f, 0.77f, 0.77f, 1.0f},
		    76
                };

	case Copper:
		return {
		    {0.25f, 0.14f, 0.14f, 1.0f},
		    {0.71f, 0.24f, 0.24f, 1.0f},
		    {0.63f, 0.06f, 0.06f, 1.0f},
		    32
                };

	case CyanPlastic:
		return {
		    {0.0f, 0.35f, 0.70f, 1.0f},
		    {0.0f,  0.5f,  1.0f, 1.0f},
		    {0.5f,  0.5f,  1.0f, 1.0f},
		    32
                };

	case Emerald:
		return {
		    { 0.0215f,   0.1745f,  0.0215f, 1.0f},
		    {0.07568f,  0.61424f, 0.07568f, 1.0f},
		    {  0.633f, 0.727811f,   0.633f, 1.0f},
		    76
                };

	case Gold:
		return {
		    {0.24725f,   0.1995f, 0.0745f, 1.0f},
		    { 0.6288f,   0.5558f, 0.3661f, 1.0f},
		    {  0.633f, 0.727811f,  0.633f, 1.0f},
		    76
                };

	case GreenPlastic:
		return {
		    {0.0f, 0.5f, 0.0f, 1.0f},
		    {0.0f, 1.0f, 0.0f, 1.0f},
		    {0.0f, 0.2f, 0.0f, 1.0f},
		    32
                };

	case Jade:
		return {
		    {0.0f, 0.6f, 0.2f, 1.0f},
		    {0.0f, 1.0f, 0.4f, 1.0f},
		    {0.0f, 0.8f, 0.2f, 1.0f},
		    32
                };

	case Obsidian:
		return {
		    {0.02f, 0.02f, 0.02f, 1.0f},
		    {0.01f, 0.01f, 0.01f, 1.0f},
		    {0.50f, 0.50f, 0.50f, 1.0f},
		    32
                };

	case Pearl:
		return {
		    {0.25f, 0.25f, 0.25f, 1.0f},
		    { 0.5f,  0.5f,  0.5f, 1.0f},
		    {0.70f, 0.70f, 0.70f, 1.0f},
		    32
                };

	case RedPlastic:
		return {
		    {0.0f, 0.0f, 0.0f, 1.0f},
		    {0.5f, 0.0f, 0.0f, 1.0f},
		    {0.7f, 0.0f, 0.0f, 1.0f},
		    32
                };

	case Ruby:
		return {
		    {  0.1745f,  0.01175f,  0.01175f, 1.0f},
		    { 0.61424f,  0.04136f,  0.04136f, 1.0f},
		    {0.727811f, 0.626959f, 0.626959f, 1.0f},
		    76
                };

	case Silver:
		return {
		    { 0.19225f,  0.19225f,  0.19225f, 1.0f},
		    { 0.50754f,  0.50754f,  0.50754f, 1.0f},
		    {0.508273f, 0.508273f, 0.508273f, 1.0f},
		    51
                };

	case Turquoise:
		return {
		    {0.0f, 0.5f, 0.5f, 1.0f},
		    {0.0f, 1.0f, 1.0f, 1.0f},
		    {0.5f, 0.5f, 0.5f, 1.0f},
		    32
                };

	case WhitePlastic:
		return {
		    {1.0f, 1.0f, 1.0f, 1.0f},
		    {1.0f, 1.0f, 1.0f, 1.0f},
		    {0.5f, 0.5f, 0.5f, 1.0f},
		    32
                };

	case YellowPlastic:
		return {
		    {1.0f, 1.0f, 0.0f, 1.0f},
		    {1.0f, 1.0f, 0.0f, 1.0f},
		    {0.5f, 0.5f, 0.0f, 1.0f},
		    32
                };

	default:
		throw std::invalid_argument("Unknown Material type");
	}

	return {};
}

void Material::Apply(const Material::Type &type)
{
	Apply(getSpecs(type));
}

void Material::Apply(const struct Specs &specs)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, specs.ambiental.data());
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, specs.diffuse.data());
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specs.spec.data());
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, specs.coefficient);
}

} // namespace OpenGL
