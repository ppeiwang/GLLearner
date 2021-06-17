#pragma once
#include "RenderCoreDefine.h"
#include "utilities/StringTable.h"
#include <string>
#include "logger/Logger.h"

RENDER_CORE_BEGIN

enum class ETextureType
{
	none = 0,
	texture_diffuse,
	texture_specular
};

struct Texture
{
	unsigned int id_{ 0 };

	ETextureType type_{ ETextureType::none };

	std::string path_;

	static const std::string_view& TextureTypeToString(ETextureType e);
};


RENDER_CORE_END
