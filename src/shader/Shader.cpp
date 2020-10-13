#include "Shader/Shader.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "utilities/StringTable.h"
#include "Config.h"

RENDER_CORE_BEGIN

void Shader::Use()
{
	glUseProgram(ID_);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID_, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID_, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID_, name.c_str()), value);
}

void Shader::SetFloatVec(const std::string& name, const glm::vec3& vec) const
{
	SetFloatVec(name, vec.x, vec.y, vec.z);
}

void Shader::SetFloatVec(const std::string& name, const glm::vec4& vec) const
{
	SetFloatVec(name, vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetFloatVec(const std::string& name, float f0, float f1, float f2) const
{
	glUniform3f(glGetUniformLocation(ID_, name.c_str()), f0, f1, f2);
}

void Shader::SetFloatVec(const std::string& name, float f0, float f1, float f2, float f3) const
{
	glUniform4f(glGetUniformLocation(ID_, name.c_str()), f0, f1, f2, f3);
}

void Shader::SetMatrix(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::GetBool(const std::string& name, bool& value) const
{
	int rtnv = 0;
	glGetUniformiv(ID_, glGetUniformLocation(ID_, name.c_str()), &rtnv);
	value = (bool)rtnv;
}

void Shader::GetInt(const std::string& name, int& value) const
{
	glGetUniformiv(ID_, glGetUniformLocation(ID_, name.c_str()), &value);
}

void Shader::GetFloat(const std::string& name, float& value) const
{
	glGetUniformfv(ID_, glGetUniformLocation(ID_, name.c_str()), &value);
}

bool Shader::AddDirectionLight(const Light::DirectionLight& d_light)
{
	if (m_direction_light_count < global::k_direction_light_limit)
	{
		const std::string variable_prefix = std::string{ global::k_shader_variable_direction_light.data() } + "[" + std::to_string(m_direction_light_count) + "].";

		const std::string variable_ambient = variable_prefix + global::k_shader_member_ambient.data();
		const std::string variable_diffuse = variable_prefix + global::k_shader_member_diffuse.data();
		const std::string variable_specular = variable_prefix + global::k_shader_member_specular.data();
		const std::string variable_direction = variable_prefix + global::k_shader_member_direction.data();

		SetFloatVec(variable_ambient.c_str(), d_light.GetAmbient());
		SetFloatVec(variable_diffuse.c_str(), d_light.GetDiffuse());
		SetFloatVec(variable_specular.c_str(), d_light.GetSpecular());
		SetFloatVec(variable_direction.c_str(), d_light.GetDirection());

		m_direction_light_count++;
		SetInt(global::k_shader_variable_direction_light_count.data(), m_direction_light_count);

		return true;
	}

	assert(false && "Out of range of direction lights limit");
	return false;
}

bool Shader::AddPointLight(const Light::PointLight& p_light)
{
	if (m_point_light_count < global::k_point_light_limit)
	{
		const std::string variable_prefix = std::string{ global::k_shader_variable_point_light.data() } + "[" + std::to_string(m_point_light_count) + "].";

		const std::string variable_ambient = variable_prefix + global::k_shader_member_ambient.data();
		const std::string variable_diffuse = variable_prefix + global::k_shader_member_diffuse.data();
		const std::string variable_specular = variable_prefix + global::k_shader_member_specular.data();
		const std::string variable_direction = variable_prefix + global::k_shader_member_direction.data();
		const std::string variable_position = variable_prefix + global::k_shader_member_position.data();
		const std::string variable_constant = variable_prefix + global::k_shader_member_constant.data();
		const std::string variable_linear = variable_prefix + global::k_shader_member_linear.data();
		const std::string variable_quadratic = variable_prefix + global::k_shader_member_quadratic.data();

		SetFloatVec(variable_ambient.c_str(), p_light.GetAmbient());
		SetFloatVec(variable_diffuse.c_str(), p_light.GetDiffuse());
		SetFloatVec(variable_specular.c_str(), p_light.GetSpecular());
		SetFloatVec(variable_position.c_str(), p_light.GetPosition());
		SetFloat(variable_constant.c_str(), p_light.GetConstant());
		SetFloat(variable_linear.c_str(), p_light.GetLinear());
		SetFloat(variable_quadratic.c_str(), p_light.GetQuadratic());

		m_point_light_count++;
		SetInt(global::k_shader_variable_point_light_count.data(), m_point_light_count);

		return true;
	}

	assert(false && "Out of range of point lights limit");
	return false;
}

bool Shader::AddSpotLight(const Light::SpotLight& s_light)
{
	if (m_spot_light_count < global::k_spot_light_limit)
	{
		const std::string variable_prefix = std::string{ global::k_shader_variable_spot_light.data() } + "[" + std::to_string(m_spot_light_count) + "].";

		const std::string variable_ambient = variable_prefix + global::k_shader_member_ambient.data();
		const std::string variable_diffuse = variable_prefix + global::k_shader_member_diffuse.data();
		const std::string variable_specular = variable_prefix + global::k_shader_member_specular.data();
		const std::string variable_position = variable_prefix + global::k_shader_member_position.data();
		const std::string variable_direction = variable_prefix + global::k_shader_member_direction.data();
		const std::string variable_cutoff = variable_prefix + global::k_shader_member_cutOff.data();
		const std::string variable_outer_cutoff = variable_prefix + global::k_shader_member_outerCutOff.data();

		SetFloatVec(variable_ambient.c_str(), s_light.GetAmbient());
		SetFloatVec(variable_diffuse.c_str(), s_light.GetDiffuse());
		SetFloatVec(variable_specular.c_str(), s_light.GetSpecular());
		SetFloatVec(variable_position.c_str(), s_light.GetPosition());
		SetFloatVec(variable_direction.c_str(), s_light.GetDirection());
		SetFloat(variable_cutoff.c_str(), s_light.GetCutOff());
		SetFloat(variable_outer_cutoff.c_str(), s_light.GetOuterCutOff());

		m_spot_light_count++;
		SetInt(global::k_shader_variable_spot_light_count.data(), m_spot_light_count);

		return true;
	}

	assert(false && "Out of range of spot lights limit");
	return false;
}

void Shader::ResetLightCount()
{
	m_direction_light_count = 0;
	m_point_light_count = 0;
	m_spot_light_count = 0;
}

void Shader::SetShaderInfo(const std::string& str_vs, const std::string& str_fs)
{
	m_vs_path_ = str_vs;
	m_fs_path_ = str_fs;
}

RENDER_CORE_END