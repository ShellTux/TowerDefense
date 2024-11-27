#include "OpenGL/Material.hpp"

#include <GL/gl.h>
#include <stdexcept>

namespace OpenGL {

struct Material::Specs Material::getSpecs(const Material::Type &type)
{
	switch (type) {
	case BlackPlastic:
		return {
		    {  0,   0,   0, 1},
		    {.01, .01, .01, 1},
		    { .5,  .5,  .5, 1},
		    32
                };

	case Brass:
		return {
		    {.25, .22, .16, 1},
		    {.78, .57, .11, 1},
		    {.99, .91, .81, 1},
		    27
                };

	case Bronze:
		return {
		    {.33, .22, .03, 1},
		    {.78, .57, .11, 1},
		    {.99, .91, .81, 1},
		    30
                };

	case Chrome:
		return {
		    {.25, .25, .25, 1},
		    { .4,  .4,  .4, 1},
		    {.77, .77, .77, 1},
		    76
                };

	case Copper:
		return {
		    {.25, .14, .14, 1},
		    {.71, .24, .24, 1},
		    {.63, .06, .06, 1},
		    32
                };

	case CyanPlastic:
		return {
		    { 0, .35, .7, 1},
		    { 0,  .5,  1, 1},
		    {.5,  .5,  1, 1},
		    32
                };

	case Emerald:
		return {
		    { .0215,   .1745,  .0215, 1},
		    {.07568,  .61424, .07568, 1},
		    {  .633, .727811,   .633, 1},
		    76
                };

	case Gold:
		return {
		    {.24725,   .1995, .0745, 1},
		    { .6288,   .5558, .3661, 1},
		    {  .633, .727811,  .633, 1},
		    76
                };

	case GreenPlastic:
		return {
		    {0, .5, 0, 1},
		    {0,  1, 0, 1},
		    {0, .2, 0, 1},
		    32
                };

	case Jade:
		return {
		    {0, .6, .2, 1},
		    {0,  1, .4, 1},
		    {0, .8, .2, 1},
		    32
                };

	case Obsidian:
		return {
		    {.02, .02, .02, 1},
		    {.01, .01, .01, 1},
		    { .5,  .5,  .5, 1},
		    32
                };

	case Pearl:
		return {
		    {.25, .25, .25, 1},
		    { .5,  .5,  .5, 1},
		    { .7,  .7,  .7, 1},
		    32
                };

	case RedPlastic:
		return {
		    { 0, 0, 0, 1},
		    {.5, 0, 0, 1},
		    {.7, 0, 0, 1},
		    32
                };

	case Ruby:
		return {
		    {  .1745,  .01175,  .01175, 1},
		    { .61424,  .04136,  .04136, 1},
		    {.727811, .626959, .626959, 1},
		    76
                };

	case Silver:
		return {
		    { .19225,  .19225,  .19225, 1},
		    { .50754,  .50754,  .50754, 1},
		    {.508273, .508273, .508273, 1},
		    51
                };

	case Turquoise:
		return {
		    { 0, .5, .5, 1},
		    { 0,  1,  1, 1},
		    {.5, .5, .5, 1},
		    32
                };

	case WhitePlastic:
		return {
		    { 1,  1,  1, 1},
		    { 1,  1,  1, 1},
		    {.5, .5, .5, 1},
		    32
                };

	case YellowPlastic:
		return {
		    { 1,  1, 0, 1},
		    { 1,  1, 0, 1},
		    {.5, .5, 0, 1},
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
