#include "tools/ShaderLoader.h"
#include "glad/glad.h"
#include "logger/Logger.h"
#include <sstream>
#include <fstream>
#include <iostream>

RENDER_CORE_BEGIN

Shader ShaderLoader::Load(const std::string& vertex_path, const std::string& fragment_path) const
{
	const std::string vertex_source = Read(vertex_path);

	const std::string fragment_source = Read(fragment_path);

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
		std::stringstream ss;
		ss << "{ " << vertex_path << " }" << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		Logger::Error(ss.str());
	};

	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_shader_code, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::stringstream ss;
		ss <<"{ " << fragment_path << " }"<< "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		Logger::Error(ss.str());
	};

	// shader Program
	auto ID_ = glCreateProgram();
	glAttachShader(ID_, vertex);
	glAttachShader(ID_, fragment);
	glLinkProgram(ID_);
	// print linking errors if any
	glGetProgramiv(ID_, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID_, 512, NULL, infoLog);
		std::stringstream ss;
		ss << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		Logger::Error(ss.str());
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return Shader{ ID_, vertex_path , fragment_path};
}

std::string ShaderLoader::Read(const std::string& file_path) const
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
			std::stringstream ss;
			ss << "Error: Failed to read " << file_path << std::endl;
			Logger::Error(ss.str());
		}
	}
	else
	{
		assert(false);
		std::stringstream ss;
		ss << "Error: Failed to open file " << file_path << std::endl;
		Logger::Error(ss.str());
	}

	return ss.str();
}

RENDER_CORE_END
