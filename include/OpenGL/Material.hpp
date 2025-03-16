#ifndef INCLUDE_OPENGL_MATERIAL_HPP_
#define INCLUDE_OPENGL_MATERIAL_HPP_

#include "types.hpp"

#include <array>

namespace OpenGL {

class Material {
      public:
	enum Type : u64 {
		BlackPlastic,
		Brass,
		Bronze,
		Chrome,
		Copper,
		CyanPlastic,
		Emerald,
		Gold,
		GreenPlastic,
		Jade,
		Obsidian,
		Pearl,
		RedPlastic,
		Ruby,
		Silver,
		Turquoise,
		WhitePlastic,
		YellowPlastic,
		Red,
		Orange,
		Purple,
	};

	struct Specs {
		std::array<f32, 4> ambiental;
		std::array<f32, 4> diffuse;
		std::array<f32, 4> spec;
		i32 coefficient;
	};

	Material(Material const &)       = delete;
	Material()                       = default;
	void operator=(Material const &) = delete;

	static Material &getInstance()
	{
		static Material instance;
		return instance;
	}

	static struct Specs getSpecs(const Type &type);
	static void Apply(const Type &type);
	static void Apply(const struct Specs &specs);

      private:
};

}; // namespace OpenGL

#endif // INCLUDE_OPENGL_MATERIAL_HPP_
