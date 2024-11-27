#include "Texture.hpp"

#include "AssetManager.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <filesystem>

Texture::Texture(const std::filesystem::path &path)
    : asset(AssetManager::getInstance().loadImage(path))
{
	glGenTextures(1, &renderId);
	glBindTexture(GL_TEXTURE_2D, renderId);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D,
	             0,
	             GL_RGBA8,
	             asset.width,
	             asset.height,
	             0,
	             GL_RGBA,
	             GL_UNSIGNED_BYTE,
	             asset.data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &renderId);
}

void Texture::Bind(const u8 slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, renderId);
}

void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
