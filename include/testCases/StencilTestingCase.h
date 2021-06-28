#pragma once
#include "testCases/TestCase.h"
#include "glm/glm.hpp"
#include <vector>
#include "shader/Shader.h"

namespace Test
{
	class StencilTestingCase : public TestCase
	{
	public:
		StencilTestingCase() = default;

		virtual ~StencilTestingCase()
		{

		}

		void Update() override;

		void Init() override;

		void DeInit() override;
	private:
		unsigned int m_cube_VAO_;

		unsigned int m_cube_VBO_;

		unsigned int m_plane_VAO_;

		unsigned int m_plane_VBO_;

		unsigned int m_cube_texture_;

		unsigned int m_floor_texture_;

		RenderCore::Shader m_shader_;

		RenderCore::Shader m_outline_shader_;

	};
}