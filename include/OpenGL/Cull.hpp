#ifndef INCLUDE_OPENGL_CULL_HPP_
#define INCLUDE_OPENGL_CULL_HPP_

#include "fmt/base.h"
#include "types.hpp"

#include <ostream>
#include <sstream>
#include <string>

namespace OpenGL {

class Cull {
      public:
	enum Culling : u8 {
		Front,
		Back,
		FrontBack,
		Disabled,
	};

	Cull();
	Cull(Cull &&)                 = default;
	Cull(const Cull &)            = default;
	Cull &operator=(Cull &&)      = default;
	Cull &operator=(const Cull &) = default;
	~Cull()                       = default;

	Cull &operator+=(const usize i);

	friend std::ostream &operator<<(std::ostream &os, const Cull &cull);

	void apply() const;

	Cull &setCullingFace(const enum Culling &cull);

	[[nodiscard]] std::string toString() const;

      private:
	enum Culling culling { Front };
};

} // namespace OpenGL

template <> struct fmt::formatter<OpenGL::Cull> : formatter<string_view> {
	auto format(OpenGL::Cull &cull, format_context &ctx) const
	{
		std::stringstream string;

		string << cull;

		return formatter<string_view>::format(string.str(), ctx);
	}
};

#endif // INCLUDE_OPENGL_CULL_HPP_
