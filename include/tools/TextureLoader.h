#pragma once
#include "RenderCoreDefine.h"
#include "scene/Texture.h"
#include <memory>

RENDER_CORE_BEGIN

class TextureLoader
{
public:
	static uint32_t LoadTexture(char const* path);

	static Texture LoadTexture(char const* path, ETextureType);
};

RENDER_CORE_END
