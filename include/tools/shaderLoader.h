#pragma once
#include "RenderCoreDefine.h"
#include "shader/Shader.h"
#include <string>

RENDER_CORE_BEGIN

class ShaderLoader
{
public:
	ShaderLoader() = default;

	Shader Load(const std::string& vertex_path, const std::string& fragment_path) const;

private:
	std::string Read(const std::string& file_path) const;
};

RENDER_CORE_END
