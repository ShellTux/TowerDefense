#ifndef INCLUDE_OPENGL_CULL_HPP_
#define INCLUDE_OPENGL_CULL_HPP_

#include "types.hpp"

#include <ostream>

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

      private:
	enum Culling culling { Front };
};

} // namespace OpenGL

#endif // INCLUDE_OPENGL_CULL_HPP_
