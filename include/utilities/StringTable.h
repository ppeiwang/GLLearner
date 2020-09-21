#pragma once
#include <string_view>
#include <string>

namespace global
{

	/* string_view */

	// variables

	const std::string_view k_shader_variable_direction_light{ "direction_light" };

	const std::string_view k_shader_variable_direction_light_count{ "direction_light_count" };

	const std::string_view k_shader_variable_point_light{ "point_light" };

	const std::string_view k_shader_variable_point_light_count{ "point_light_count" };

	const std::string_view k_shader_variable_spot_light{ "spot_light" };

	const std::string_view k_shader_variable_spot_light_count{ "spot_light_count" };

	// members

	const std::string_view k_shader_member_ambient{"ambient"};

	const std::string_view k_shader_member_diffuse{ "diffuse" };

	const std::string_view k_shader_member_specular{ "specular" };

	const std::string_view k_shader_member_direction{ "direction" };

	const std::string_view k_shader_member_position{ "position" };

	const std::string_view k_shader_member_constant{ "constant" };

	const std::string_view k_shader_member_linear{ "linear" };

	const std::string_view k_shader_member_quadratic{ "quadratic" };

	const std::string_view k_shader_member_cutOff{ "cutOff" };


}

