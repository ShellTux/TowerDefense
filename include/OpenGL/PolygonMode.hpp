#ifndef INCLUDE_OPENGL_POLYGONMODE_HPP_
#define INCLUDE_OPENGL_POLYGONMODE_HPP_

#include "types.hpp"

#include <ostream>

namespace OpenGL {

class PolygonMode {
      public:
	enum Type : u8 {
		Point,
		Line,
		Fill,
	};

	PolygonMode();
	PolygonMode(PolygonMode &&)                 = default;
	PolygonMode(const PolygonMode &)            = default;
	PolygonMode &operator=(PolygonMode &&)      = default;
	PolygonMode &operator=(const PolygonMode &) = default;
	~PolygonMode()                              = default;

	PolygonMode &operator+=(const usize i);

	friend std::ostream &operator<<(std::ostream &os,
	                                const PolygonMode &polygonMode);

	void apply() const;

      private:
	enum Type mode { Fill };
};

} // namespace OpenGL

#endif // INCLUDE_OPENGL_POLYGONMODE_HPP_
