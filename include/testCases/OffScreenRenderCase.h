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
	void DrawMesh();

private:
	unsigned int m_cube_VAO_{ 0 };

	unsigned int m_cube_VBO_{ 0 };

	unsigned int m_cube_texture_{ 0 };

	unsigned int m_plane_VAO_{ 0 };

	unsigned int m_plane_VBO_{ 0 };

	unsigned int m_plane_texture_{ 0 };

	unsigned int m_quad_VAO_{ 0 };

	unsigned int m_quad_VBO_{ 0 };

	unsigned int m_quad_texture_{ 0 };

	unsigned int m_frame_buffer_{ 0 };

	unsigned int m_texture_color_buffer_{ 0 };

	RenderCore::Shader m_shader_{};

	RenderCore::Shader m_screen_shader_{};
};
}