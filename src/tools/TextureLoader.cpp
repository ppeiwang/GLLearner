#include "tools/TextureLoader.h"
#include "glad/glad.h"
#include "stb/stb_image.h"
#include "logger/Logger.h"
#include "utilities/Exception.h"

RENDER_CORE_BEGIN

std::unordered_map<std::string, uint32_t> TextureLoader::gLoadedTextureMap; //<path, ID>

uint32_t TextureLoader::LoadTextureImp(char const* path)
{
	if (!path)
	{
		assert(false && "the pointer of path is null");
		throw std::runtime_error("LoadTexture from a null path");
	}

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RGBA;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		Logger::Error("Texture failed to load at path:" + std::string(path));
		stbi_image_free(data);
	}

	return textureID;
}

Texture TextureLoader::LoadTexture(char const* path, ETextureType texture_type)
{
	if (auto itr = gLoadedTextureMap.find(path); itr != gLoadedTextureMap.end()) {
		const auto textureID = itr->second;
		return { textureID, texture_type, path };
	}
	else {
		try
		{
			const uint32_t textureID = LoadTextureImp(path);
			gLoadedTextureMap.emplace(std::string(path), textureID);
			return { textureID, texture_type, path };
		}
		catch (const std::runtime_error& e)
		{
			Logger::Error(std::string("Failed to load texture: ") + e.what());
			return {};
		}
	}
}

RENDER_CORE_END
