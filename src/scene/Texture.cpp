#include "scene/Texture.h"

RENDER_CORE_BEGIN

const std::string_view& Texture::TextureTypeToString(ETextureTye e)
{
	switch (e)
	{
	case RenderCore::ETextureTye::texture_diffuse:
		return global::k_shader_member_diffuse;
	case RenderCore::ETextureTye::texture_specular:
		return global::k_shader_member_specular;
	default:
		return global::k_shader_empty;
	}
}

RENDER_CORE_END


