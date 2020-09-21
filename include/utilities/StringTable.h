#pragma once
#include <string_view>
#include <string>

namespace global
{

	/* string_view */

	// variables

	const std::string_view k_shader_variable_direction_light{ "direction_lights" };

	const std::string_view k_shader_variable_direction_light_count{ "direction_lights_count" };

	const std::string_view k_shader_variable_point_light{ "point_lights" };

	const std::string_view k_shader_variable_point_light_count{ "point_lights_count" };

	const std::string_view k_shader_variable_spot_light{ "spot_lights" };

	const std::string_view k_shader_variable_spot_light_count{ "spot_lights_count" };

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

	const std::string_view k_shader_member_outerCutOff{ "outerCutOff" };

}

