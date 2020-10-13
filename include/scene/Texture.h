#pragma once
#include "RenderCoreDefine.h"
#include "utilities/StringTable.h"
#include <string>
#include "logger/Logger.h"

RENDER_CORE_BEGIN

enum class ETextureTye
{
	none = 0,
	texture_diffuse,
	texture_specular
};

struct Texture 
{
	unsigned int id_{0};

	ETextureTye type_{ETextureTye::none};

	static const std::string_view& TextureTypeToString(ETextureTye e);
};


RENDER_CORE_END
