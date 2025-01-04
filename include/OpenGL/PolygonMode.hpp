#ifndef INCLUDE_OPENGL_POLYGONMODE_HPP_
#define INCLUDE_OPENGL_POLYGONMODE_HPP_

#include "fmt/base.h"
#include "types.hpp"

#include <ostream>
#include <sstream>
#include <string>

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

        [[nodiscard]] std::string toString() const;

      private:
	enum Type mode { Fill };
};

} // namespace OpenGL

template <> struct fmt::formatter<OpenGL::PolygonMode> : formatter<string_view> {
    auto format(OpenGL::PolygonMode &polygon, format_context &ctx) const {
        std::stringstream string;

        string << polygon;

        return formatter<string_view>::format(string.str(), ctx);
    }
};

#endif // INCLUDE_OPENGL_POLYGONMODE_HPP_
