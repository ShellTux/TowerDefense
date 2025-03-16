#include "Debug.hpp"

#include <iostream>

namespace Debug {

std::ostream &operator<<(std::ostream &os, const Level &level)
{
	switch (level) {
	case Level::INFO: {
		os << "\033[36m";
		os << "  INFO   ";
		os << "\033[0m";
	} break;
	case Level::WARNING: {
		os << "\033[33m";
		os << " WARNING ";
		os << "\033[0m";
	} break;
	case Level::EVENT: {
		os << "\033[34m";
		os << "  EVENT  ";
		os << "\033[0m";
	} break;
	case Level::ERROR: {
		os << "\033[31m";
		os << "  ERROR  ";
		os << "\033[0m";
	} break;
	}

	return os;
}

} // namespace Debug
