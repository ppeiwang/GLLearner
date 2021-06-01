#include "testCases/StencilTestingCase.h"
#include "glad/glad.h"
#include "tools/shaderLoader.h"
#include "tools/TextureLoader.h"
#include "scene/Scene.h"
#include "camera/Camera.h"
#include "geometry/basic_geometry/BasicGeometry.h"

using namespace RenderCore;

namespace Test
{
	void StencilTestingCase::Init()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		const float* cubeVertices = basic_gemotry::cubeVertices;

		// cube VAO
		glGenVertexArrays(1, &m_cube_VAO_);
		glGenBuffers(1, &m_cube_VBO_);
		glBindVertexArray(m_cube_VAO_);
		glBindBuffer(GL_ARRAY_BUFFER, m_cube_VBO_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		const float* planeVertices = basic_gemotry::planeVertices;
		// plane VAO
		glGenVertexArrays(1, &m_plane_VAO_);
		glGenBuffers(1, &m_plane_VBO_);
		glBindVertexArray(m_plane_VAO_);
		glBindBuffer(GL_ARRAY_BUFFER, m_plane_VBO_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		// load textures
		m_cube_texture_ = TextureLoader::LoadTexture("assets/texture/dalishi.jpg", ETextureType::none).id_;
		m_floor_texture_ = TextureLoader::LoadTexture("assets/texture/jinshu.jpg", ETextureType::none).id_;

		// shader configuration
		ShaderLoader shader_loader;

		m_outline_shader_ = shader_loader.Load(R"(assets/shader/vs_texture.glsl)", R"(assets/shader/fs_outline.glsl)");
		m_shader_ = shader_loader.Load(R"(assets/shader/vs_texture.glsl)", R"(assets/shader/fs_stencil_testing.glsl)");
		m_shader_.Use();
		m_shader_.SetInt("texture1", 0);
	}

	void StencilTestingCase::Update()
	{
		glClearColor(0.15f, 0.15f, 0.18f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		std::shared_ptr<Camera> ptr_camera = m_scene_instance_->GetCamera();

		Camera& camera_instance_ = *ptr_camera;

		m_outline_shader_.Use();
		m_outline_shader_.SetMatrix("projection", camera_instance_.GetProjectMatrix());
		m_outline_shader_.SetMatrix("view", camera_instance_.GetViewMatrix());

		m_shader_.SetMatrix("projection", camera_instance_.GetProjectMatrix());
		m_shader_.SetMatrix("view", camera_instance_.GetViewMatrix());

		glStencilMask(0x00);
		// floor
		glBindVertexArray(m_plane_VAO_);
		glBindTexture(GL_TEXTURE_2D, m_floor_texture_);
		m_shader_.SetMatrix("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);


		// 1st. render pass, draw objects as normal, writing to the stencil buffer
		// --------------------------------------------------------------------
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		// cubes
		glBindVertexArray(m_cube_VAO_);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_cube_texture_);
		auto model = glm::translate(glm::mat4{ 1.0f }, glm::vec3(-1.0f, 0.0f, -1.0f));
		m_shader_.SetMatrix("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		m_shader_.SetMatrix("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
		// Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
		// the objects' size differences, making it look like borders.
		// -----------------------------------------------------------------------------------------------------------------------------
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		m_outline_shader_.Use();
		float scale = 1.1;
		// cubes
		glBindVertexArray(m_cube_VAO_);
		glBindTexture(GL_TEXTURE_2D, m_cube_texture_);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		m_outline_shader_.SetMatrix("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		m_outline_shader_.SetMatrix("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
	}

	void StencilTestingCase::DeInit()
	{
		glDeleteVertexArrays(1, &m_cube_VAO_);
		glDeleteVertexArrays(1, &m_plane_VAO_);
		glDeleteBuffers(1, &m_cube_VBO_);
		glDeleteBuffers(1, &m_plane_VBO_);
	}

}
