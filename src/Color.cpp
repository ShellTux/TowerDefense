#include "Color.hpp"

#include "types.hpp"

#include <ostream>

std::ostream &operator<<(std::ostream &os, const Color &color)
{
	const u8 r = color.at(0);
	const u8 g = color.at(1);
	const u8 b = color.at(2);
	os << "\033[38;2;" << r << ";" << g << ";" << b << "m";
	return os;
}
