#include "Texture.hpp"

#include "types.hpp"

#include <GL/gl.h>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <vector>

extern bool texturesNeedReload;

std::unordered_map<std::filesystem::path, Texture> Texture::GlobalTextures = {};

Texture::Texture(const std::filesystem::path &path)
    : path(path)
{
	glGenTextures(1, &renderId);

	reload();
}

Texture::~Texture()
{
	glDeleteTextures(1, &renderId);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::ReloadAll()
{
	std::cout << "Reloading all textures" << std::endl;

	for (const auto &[path, texture] : GlobalTextures) {
		texture.reload();
	}

	texturesNeedReload = false;

	std::cout << "Global Textures: " << GlobalTextures << std::endl;
}

void Texture::Load(const std::filesystem::path &path)
{
	if (GlobalTextures.find(path) != GlobalTextures.end()) {
		return;
	}

	std::cout << "Loading new texture: " << path << std::endl;
	GlobalTextures[path] = Texture(path);
}

void Texture::Loads(const std::vector<std::filesystem::path> &paths)
{
	for (const std::filesystem::path &path : paths) {
		Load(path);
	}
}

Texture &Texture::Get(const std::filesystem::path &path)
{
	Load(path);

	if (texturesNeedReload) {
		ReloadAll();
	}

	return GlobalTextures.at(path);
}


void Texture::Bind(const u8 slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, renderId);
}

void Texture::Unbind() const
{
	UnBind();
}

void Texture::reload() const
{
	struct Image image(path);

	Bind();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D,
	             0,
	             GL_RGBA8,
	             image.width,
	             image.height,
	             0,
	             GL_RGBA,
	             GL_UNSIGNED_BYTE,
	             image.data);

	Unbind();
}

GLuint Texture::getRenderId() const
{
	return renderId;
}

std::filesystem::path Texture::getPath() const
{
	return path;
}
