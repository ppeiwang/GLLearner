#pragma once
#include "RenderCoreDefine.h"
#include <memory>

RENDER_CORE_BEGIN

class TextureLoader
{
public:
	static uint32_t LoadTexture(char const* path);
};

RENDER_CORE_END
