#include <sstream>
#include <fstream>
#include <iostream>
#include "tools/shaderLoader.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "utilities/StringTable.h"

ShaderLoader::ShaderLoader(const std::string& vertex_path, const std::string& fragment_path)
{
	const std::string vertex_source = ShaderLoader::Read(vertex_path);

	const std::string fragment_source = ShaderLoader::Read(fragment_path);

	const char* vertext_shader_code = vertex_source.c_str();

	const char* fragment_shader_code = fragment_source.c_str();

	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex, 1, &vertext_shader_code, NULL);

	glCompileShader(vertex);

	int success;

	char infoLog[512];

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cerr << "{ " << vertex_path << " }" << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_shader_code, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cerr  <<"{ " << fragment_path << " }"<< "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// shader Program
	ID_ = glCreateProgram();
	glAttachShader(ID_, vertex);
	glAttachShader(ID_, fragment);
	glLinkProgram(ID_);
	// print linking errors if any
	glGetProgramiv(ID_, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID_, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void ShaderLoader::Use()
{
	glUseProgram(ID_);
}

void ShaderLoader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID_, name.c_str()), (int)value);
}

void ShaderLoader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID_, name.c_str()), value);
}

void ShaderLoader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID_, name.c_str()), value);
}

void ShaderLoader::SetFloatVec(const std::string& name, const glm::vec3& vec) const
{
	SetFloatVec(name, vec.x, vec.y, vec.z);
}

void ShaderLoader::SetFloatVec(const std::string& name, const glm::vec4& vec) const
{
	SetFloatVec(name, vec.x, vec.y, vec.z, vec.w);
}

void ShaderLoader::SetFloatVec(const std::string& name, float f0, float f1, float f2) const
{
	glUniform3f(glGetUniformLocation(ID_, name.c_str()), f0, f1, f2);
}

void ShaderLoader::SetFloatVec(const std::string& name, float f0, float f1, float f2, float f3) const
{
	glUniform4f(glGetUniformLocation(ID_, name.c_str()), f0, f1, f2, f3);
}

void ShaderLoader::SetMatrix(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderLoader::GetBool(const std::string& name, bool& value) const
{
	int rtnv = 0;
	glGetUniformiv(ID_, glGetUniformLocation(ID_, name.c_str()), &rtnv);
	value = (bool)rtnv;
}

void ShaderLoader::GetInt(const std::string& name, int& value) const
{
	glGetUniformiv(ID_, glGetUniformLocation(ID_, name.c_str()), &value);
}

void ShaderLoader::GetFloat(const std::string& name, float& value) const
{
	glGetUniformfv(ID_, glGetUniformLocation(ID_, name.c_str()), &value);
}

std::string ShaderLoader::Read(const std::string& file_path)
{
	std::stringstream ss;
	std::ifstream file(file_path, std::ios::in | std::ios::binary);
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	if (file.is_open())
	{
		try {
			ss << file.rdbuf();
		}
		catch (std::ifstream::failure e)
		{
			assert(false);
			std::cerr << "Error: Failed to read " << file_path << std::endl;
		}
	}
	else
	{
		assert(false);
		std::cerr << "Error: Failed to open file " << file_path << std::endl;
	}

	return ss.str();
}

bool ShaderLoader::AddDirectionLight(const Light::DirectionLight& d_light)
{
	if (m_direction_light_count < global::k_direction_light_limit)
	{
		m_direction_light_count++;

		const std::string variable_prefix = std::string{ global::k_shader_variable_direction_light.data()} + "[" + std::to_string(m_direction_light_count) + "].";

		const std::string variable_ambient = variable_prefix + global::k_shader_member_ambient.data();
		const std::string variable_diffuse = variable_prefix + global::k_shader_member_diffuse.data();
		const std::string variable_specular = variable_prefix + global::k_shader_member_specular.data();
		const std::string variable_direction = variable_prefix + global::k_shader_member_direction.data();

		SetInt(global::k_shader_variable_direction_light_count.data(), m_direction_light_count);
		SetFloatVec(variable_ambient.c_str(), d_light.GetAmbient());
		SetFloatVec(variable_diffuse.c_str(), d_light.GetDiffuse());
		SetFloatVec(variable_specular.c_str(), d_light.GetSpecular());
		SetFloatVec(variable_direction.c_str(), d_light.GetDirection());

		return true;
	}

	assert(false && "Out of range of direction lights limit");
	return false;
}

bool ShaderLoader::AddPointLight(const Light::PointLight& p_light)
{
	if (m_point_light_count < global::k_point_light_limit)
	{
		m_point_light_count++;

		const std::string variable_prefix = std::string{ global::k_shader_variable_point_light.data() } + "[" + std::to_string(m_point_light_count) + "].";

		const std::string variable_ambient = variable_prefix + global::k_shader_member_ambient.data();
		const std::string variable_diffuse = variable_prefix + global::k_shader_member_diffuse.data();
		const std::string variable_specular = variable_prefix + global::k_shader_member_specular.data();
		const std::string variable_direction = variable_prefix + global::k_shader_member_direction.data();
		const std::string variable_position = variable_prefix + global::k_shader_member_position.data();
		const std::string variable_constant = variable_prefix + global::k_shader_member_constant.data();
		const std::string variable_linear = variable_prefix + global::k_shader_member_linear.data();
		const std::string variable_quadratic = variable_prefix + global::k_shader_member_quadratic.data();

		SetInt(global::k_shader_variable_point_light_count.data(), m_point_light_count);
		SetFloatVec(variable_ambient.c_str(), p_light.GetAmbient());
		SetFloatVec(variable_diffuse.c_str(), p_light.GetDiffuse());
		SetFloatVec(variable_specular.c_str(), p_light.GetSpecular());
		SetFloatVec(variable_position.c_str(), p_light.GetPosition());
		SetFloat(variable_constant.c_str(), p_light.GetConstant());
		SetFloat(variable_linear.c_str(), p_light.GetLinear());
		SetFloat(variable_quadratic.c_str(), p_light.GetQuadratic());

		return true;
	}

	assert(false && "Out of range of point lights limit");
	return false;
}

bool ShaderLoader::AddSpotLight(const Light::SpotLight& s_light)
{
	if (m_spot_light_count < global::k_spot_light_limit)
	{
		m_spot_light_count++;

		const std::string variable_prefix = std::string{ global::k_shader_variable_spot_light.data() } + "[" + std::to_string(m_spot_light_count) + "].";

		const std::string variable_ambient = variable_prefix + global::k_shader_member_ambient.data();
		const std::string variable_diffuse = variable_prefix + global::k_shader_member_diffuse.data();
		const std::string variable_specular = variable_prefix + global::k_shader_member_specular.data();
		const std::string variable_position = variable_prefix + global::k_shader_member_position.data();
		const std::string variable_direction = variable_prefix + global::k_shader_member_direction.data();
		const std::string variable_cutoff = variable_prefix + global::k_shader_member_cutOff.data();

		SetInt(global::k_shader_variable_spot_light_count.data(), m_spot_light_count);
		SetFloatVec(variable_ambient.c_str(), s_light.GetAmbient());
		SetFloatVec(variable_diffuse.c_str(), s_light.GetDiffuse());
		SetFloatVec(variable_specular.c_str(), s_light.GetSpecular());
		SetFloatVec(variable_position.c_str(), s_light.GetPosition());
		SetFloatVec(variable_direction.c_str(), s_light.GetDirection());
		SetFloat(variable_cutoff.c_str(), s_light.GetCutoff());

		return true;
	}

	assert(false && "Out of range of spot lights limit");
	return false;
}

