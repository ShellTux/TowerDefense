#include "Color.hpp"

#include "types.hpp"

#ifndef NOOF
	#include "ofMain.h"
#endif

#include <GL/gl.h>
#include <ostream>

void glColor(const u32 color)
{
	const Color c = ((color & 0xFF000000) != 0)
	                    ? Color{u8((color & 0xFF000000) >> 24),
	                            u8((color & 0x00FF0000) >> 16),
	                            u8((color & 0x0000FF00) >> 8),
	                            u8((color & 0x000000FF) >> 0)}
	                    : Color{
	                          u8((color & 0xFF0000) >> 16),
	                          u8((color & 0x00FF00) >> 8),
	                          u8((color & 0x0000FF) >> 0),
	                          u8(0xFF),
	                      };

	glColor4ubv(c.data());
}

std::ostream &operator<<(std::ostream &os, const Color &color)
{
	const u8 r = color.at(0);
	const u8 g = color.at(1);
	const u8 b = color.at(2);
	os << "\033[38;2;" << r << ";" << g << ";" << b << "m";
	return os;
}
