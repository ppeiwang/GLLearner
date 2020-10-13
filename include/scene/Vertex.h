#pragma once
#include "RenderCoreDefine.h"
#include "glm/glm.hpp"


RENDER_CORE_BEGIN

// The layout is depended
struct Vertex
{
	glm::vec3 position_ {0.0f, 0.0f, 0.0f};
	glm::vec3 normal_ { 0.0f, 0.0f, 0.0f };
	glm::vec2 texture_coords_ { 0.0f, 0.0f};
};

RENDER_CORE_END
