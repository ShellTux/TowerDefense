#ifndef INCLUDE_INCLUDE_ASSETMANAGER_HPP_
#define INCLUDE_INCLUDE_ASSETMANAGER_HPP_

#include "stb_image.h"
#include "types.hpp"

#include <array>
#include <filesystem>
#include <unordered_map>

class AssetManager {
      public:
	AssetManager(AssetManager const &)   = delete;
	AssetManager()                       = default;
	void operator=(AssetManager const &) = delete;

	struct ImageAsset {
		u8 *data;
		i32 width;
		i32 height;
		i32 channels;

		[[nodiscard]] std::array<f64, 4> getAverageColor() const;
		[[nodiscard]] u8 *getPixel(u32 row, u32 col) const;
	};

	static AssetManager &getInstance()
	{
		static AssetManager instance;
		return instance;
	}

	struct ImageAsset loadImage(const std::filesystem::path &filePath);
	void freeAssets();

      private:
	std::unordered_map<std::filesystem::path, ImageAsset> assets;
};

#endif // INCLUDE_INCLUDE_ASSETMANAGER_HPP_
