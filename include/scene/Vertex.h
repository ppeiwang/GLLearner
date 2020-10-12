#pragma once
#include "RenderCoreDefine.h"
#include "glm/glm.hpp"


RENDER_CORE_BEGIN

struct Vertex
{
	glm::vec3 position_;
	glm::vec3 normal_;
	glm::vec2 texture_coords_;
};

RENDER_CORE_END
