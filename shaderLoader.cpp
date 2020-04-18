#include "shaderLoader.h"
#include <sstream>

ShaderLoader::ShaderLoader(const std::string& file_name):
	file_name_(file_name)
{

}

std::string ShaderLoader::DumpShader() const
{
	std::stringstream ss;
	std::ifstream file(file_name_, std::ios::in|std::ios::binary);
	if (file.is_open())
	{
		ss << file.rdbuf();
	}

	return ss.str();
}
