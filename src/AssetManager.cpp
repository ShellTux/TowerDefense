#include "AssetManager.hpp"

#include "types.hpp"

#include <array>
#include <filesystem>
#include <iostream>

std::array<f64, 4> AssetManager::ImageAsset::getAverageColor() const
{
	u64 r = 0;
	u64 g = 0;
	u64 b = 0;
	u64 a = 0;

	for (i32 i = 0; i < height; ++i) {
		for (i32 j = 0; j < width; ++j) {
			const u8 *const pixel = getPixel(i, j);

			r += pixel[0];
			g += pixel[1];
			b += pixel[2];
			a += pixel[3];
		}
	}

	const u64 totalPixels = u64(width) * u64(height);

	return {f64(r) / f64(totalPixels),
	        f64(g) / f64(totalPixels),
	        f64(b) / f64(totalPixels),
	        f64(a) / f64(totalPixels)};
}

u8 *AssetManager::ImageAsset::getPixel(u32 row, u32 col) const
{
	if (row < 0 || i32(row) >= height || col < 0 || i32(col) >= width) {
		std::cerr << "Coordinates out of bounds!" << std::endl;
		return nullptr;
	}

	return data + (row * width + col) * channels;
}

struct AssetManager::ImageAsset
AssetManager::loadImage(const std::filesystem::path &filePath)
{
	if (assets.find(filePath) != assets.end()) {
		return assets[filePath];
	}

	u8 *data     = nullptr;
	i32 width    = 0;
	i32 height   = 0;
	i32 channels = 0;

	data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

	if (data == nullptr) {
		std::cerr << "Error loading image: " << stbi_failure_reason()
		          << std::endl;
		return {};
	}

	assets[filePath] = ImageAsset{
	    .data     = data,
	    .width    = width,
	    .height   = height,
	    .channels = channels,
	};

	return assets[filePath];
}

void AssetManager::freeAssets()
{
	for (auto &pair : assets) {
		if (pair.second.data != nullptr) {
			stbi_image_free(pair.second.data);
			pair.second.data = nullptr;
		}
	}
	assets.clear();
}
