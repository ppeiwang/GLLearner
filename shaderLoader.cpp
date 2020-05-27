#include <sstream>
#include <fstream>
#include <iostream>
#include "shaderLoader.h"
#include "glad/glad.h"

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
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_shader_code, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
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
	glUniform3f(glGetUniformLocation(ID_, name.c_str()), vec.x, vec.y, vec.z);
}

void ShaderLoader::SetFloatVec(const std::string& name, const glm::vec4& vec) const
{
	glUniform4f(glGetUniformLocation(ID_, name.c_str()), vec.x, vec.y, vec.z, vec.w);
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

