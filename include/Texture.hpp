#ifndef INCLUDE_INCLUDE_TEXTURE_HPP_
#define INCLUDE_INCLUDE_TEXTURE_HPP_

#include "AssetManager.hpp"
#include "types.hpp"

#include <filesystem>

class Texture {
      public:
	Texture(const std::filesystem::path &path);
	Texture(Texture &&)                 = default;
	Texture(const Texture &)            = default;
	Texture &operator=(Texture &&)      = default;
	Texture &operator=(const Texture &) = default;
	~Texture();

	void Bind(const u8 slot = 0) const;
	void UnBind() const;

      private:
	AssetManager::ImageAsset asset;

	u32 renderId{};
};

#endif // INCLUDE_INCLUDE_TEXTURE_HPP_
