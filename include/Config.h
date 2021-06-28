#pragma once
#include "RenderCoreDefine.h"

RENDER_CORE_BEGIN

namespace global
{
// the max count of direction light a shader used
const unsigned int k_direction_light_limit = 8;

// the max count of spot light a shader used
const unsigned int k_spot_light_limit = 16;

// the max count of point light a shader used
const unsigned int k_point_light_limit = 32;

// the max count of texture diffuse map a shader used
const unsigned int k_texture_diffuse_limit = 1;

// the max count of texture specular map a shader used
const unsigned int k_texture_specular_limit = 1;

constexpr int k_window_width = 800;

constexpr int k_window_height = 600;
}

RENDER_CORE_END
