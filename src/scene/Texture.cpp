#include "scene/Texture.h"
#include "utilities/Exception.h"

RENDER_CORE_BEGIN

const std::string_view& Texture::TextureTypeToString(ETextureType e)
{
	switch (e)
	{
		case RenderCore::ETextureType::texture_diffuse:
			return global::k_shader_member_diffuse;
		case RenderCore::ETextureType::texture_specular:
			return global::k_shader_member_specular;
		default:
		{
			throw std::runtime_error{ "Bad ETextureType" };
		}
	}
}

RENDER_CORE_END


