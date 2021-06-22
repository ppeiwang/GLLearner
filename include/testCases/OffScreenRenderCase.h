#pragma once
#include "testCases/TestCase.h"
#include "glm/glm.hpp"
#include <vector>
#include "shader/Shader.h"

namespace Test
{
class OffScreenRenderCase : public TestCase
{
public:
	OffScreenRenderCase() = default;

	virtual ~OffScreenRenderCase()
	{
	}

	void Update() override;

	void Init() override;

	void DeInit() override;

private:
	unsigned int m_cube_VAO_{ 0 };

	unsigned int m_cube_VBO_{ 0 };

	unsigned int m_cube_texture_{ 0 };

	unsigned int m_framebuffer_{ 0 };

	unsigned int m_texture_buffer_{ 0 };

	RenderCore::Shader m_shader_{};
};
}