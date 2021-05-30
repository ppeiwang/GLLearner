#pragma once
#include "RenderCoreDefine.h"
#include "scene/Texture.h"
#include <memory>
#include <unordered_map>

RENDER_CORE_BEGIN

class TextureLoader
{
public:

	static Texture LoadTexture(char const* path, ETextureType);

	static void SetFlipVertically(bool flag)
	{
		gFlipVertically = flag;
	}

private:
	static uint32_t LoadTextureImp(char const* path);

	static bool gFlipVertically;

	static std::unordered_map<std::string, uint32_t> gLoadedTextureMap; //<path, ID>
};

RENDER_CORE_END
