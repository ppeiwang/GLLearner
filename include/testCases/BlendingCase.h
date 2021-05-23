#pragma once
#include "testCases/TestCase.h"
#include "glm/glm.hpp"
#include <vector>
#include "shader/Shader.h"

namespace Test
{
	class BlendingCase : public TestCase
	{
	public:
		BlendingCase() = default;

		virtual ~BlendingCase()
		{

		}

		void Update() override;

		void Init() override;

		void DeInit() override;
	private:
		std::vector<glm::vec3> m_vegetation_position_;

		unsigned int m_cube_VAO_;

		unsigned int m_cube_VBO_;

		unsigned int m_plane_VAO_;

		unsigned int m_plane_VBO_;

		unsigned int m_transparent_VAO_;

		unsigned int m_transparent_VBO_;

		unsigned int m_cube_texture_;

		unsigned int m_floor_texture_;

		unsigned int m_transparent_texture_;

		RenderCore::Shader m_shader_;

	};
}