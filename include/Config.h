#pragma once

namespace RenderCore
{
	namespace global
	{
		// the max count of direction light a shader used
		const unsigned int k_direction_light_limit = 8;

		// the max count of spot light a shader used
		const unsigned int k_spot_light_limit = 16;

		// the max count of point light a shader used
		const unsigned int k_point_light_limit = 32;

	}
}
