#ifndef INCLUDE_INCLUDE_TEXTURE_HPP_
#define INCLUDE_INCLUDE_TEXTURE_HPP_

#include "Color.hpp"
#include "stb_image.h"
#include "types.hpp"

#ifndef NOOF
	#include "ofMain.h"
#endif

#include <GL/gl.h>
#include <filesystem>
#include <ostream>
#include <unordered_map>
#include <vector>

template <> class std::hash<std::filesystem::path> {
      public:
	size_t operator()(const std::filesystem::path &path) const
	{
		return std::hash<std::string>{}(path.string());
	}
};

class Texture {
      public:
	struct Image {
		u8 *data = nullptr;
		i32 width{};
		i32 height{};
		i32 channels{};

		[[nodiscard]] Color getAverageColor() const;
		[[nodiscard]] u8 *getPixel(u32 row, u32 col) const;

		Image()                         = delete;
		Image(const Image &)            = default;
		Image(Image &&)                 = delete;
		Image &operator=(const Image &) = default;
		Image &operator=(Image &&)      = delete;

		Image(const std::filesystem::path &path)
		{
			data = stbi_load(path.string().c_str(),
			                 &width,
			                 &height,
			                 &channels,
			                 0);
		}

		~Image()
		{
			stbi_image_free(data);
			data = nullptr;
		}
	};


	Texture()                           = default;
	Texture(Texture &&)                 = default;
	Texture(const Texture &)            = default;
	Texture &operator=(Texture &&)      = default;
	Texture &operator=(const Texture &) = default;
	~Texture();

	static void UnBind();
	static void ReloadAll();
	static void Load(const std::filesystem::path &path);
	static void Loads(const std::vector<std::filesystem::path> &paths);
	static Texture &Get(const std::filesystem::path &path);

	static std::unordered_map<std::filesystem::path, Texture>
	    GlobalTextures;

	friend std::ostream &operator<<(
	    std::ostream &os,
	    const std::unordered_map<std::filesystem::path, Texture> &textures);

	void Bind(const u8 slot = 0) const;
	void Unbind() const;

	void reload() const;

#ifdef NOOF
	[[nodiscard]] GLuint getRenderId() const;
#endif
	[[nodiscard]] std::filesystem::path getPath() const;

      private:
	Texture(const std::filesystem::path &path);

#ifdef NOOF
	GLuint renderId{};
#else
	ofImage image;
#endif
	std::filesystem::path path;
};

#endif // INCLUDE_INCLUDE_TEXTURE_HPP_
