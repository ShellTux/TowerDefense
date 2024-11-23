#ifndef INCLUDE_INCLUDE_DEBUG_HPP_
#define INCLUDE_INCLUDE_DEBUG_HPP_

#include <ostream>
#if 0
	#define DEBUG
#endif

namespace Debug {

enum class Level {
	INFO,
	WARNING,
	EVENT,
	ERROR,
};

std::ostream &operator<<(std::ostream &os, const Level &level);

} // namespace Debug

#ifdef DEBUG
	#define DEBUG_PRINT(LEVEL, ...)                                \
		std::cerr << "[" << LEVEL                              \
		          << "]"                                       \
		             " ["                                      \
		          << __FUNCTION__ << "(), " << __FILE__ << ":" \
		          << __LINE__ << "] " << __VA_ARGS__ << std::endl;
#else
	#define DEBUG_PRINT(...)
#endif

#endif // INCLUDE_INCLUDE_DEBUG_HPP_
