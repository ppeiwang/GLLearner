#include "testCases/BlendingCase.h"
#include "glad/glad.h"
#include "tools/shaderLoader.h"
#include "tools/TextureLoader.h"
#include "scene/Scene.h"
#include "camera/Camera.h"

using namespace RenderCore;

namespace Test
{
	void BlendingCase::Update()
	{
		m_shader_.Use();

		std::shared_ptr<Camera> ptr_camera = m_scene_instance_->GetCamera();

		Camera& camera_instance_ = *ptr_camera;

		m_shader_.SetMatrix("projection", camera_instance_.GetProjectMatrix());
		m_shader_.SetMatrix("view", camera_instance_.GetViewMatrix());
		// cubes
		glBindVertexArray(m_cube_VAO_);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_cube_texture_);
		glm::mat4 model{ 1.0f };
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		m_shader_.SetMatrix("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		m_shader_.SetMatrix("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// floor
		glBindVertexArray(m_plane_VAO_);
		glBindTexture(GL_TEXTURE_2D, m_floor_texture_);
		model = glm::mat4(1.0f);
		m_shader_.SetMatrix("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < m_vegetation_position_.size(); i++)
		{
			float distance = glm::length(camera_instance_.GetPosition() - m_vegetation_position_[i]);
			sorted[distance] = m_vegetation_position_[i];
		}

		glBindVertexArray(m_transparent_VAO_);
		glBindTexture(GL_TEXTURE_2D, m_transparent_window_);
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);
			m_shader_.SetMatrix("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

	}

	void BlendingCase::Init()
	{
		m_vegetation_position_.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
		m_vegetation_position_.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
		m_vegetation_position_.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
		m_vegetation_position_.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
		m_vegetation_position_.push_back(glm::vec3(0.5f, 0.0f, -0.6f));


		float cubeVertices[] = {
			// positions          // texture Coords
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		float planeVertices[] = {
			// positions          // texture Coords 
			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
			 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
		};
		float transparentVertices[] = {
			// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
			0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
			0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

			0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
			1.0f,  0.5f,  0.0f,  1.0f,  0.0f
		};
		// cube VAO
		glGenVertexArrays(1, &m_cube_VAO_);
		glGenBuffers(1, &m_cube_VAO_);
		glBindVertexArray(m_cube_VAO_);
		glBindBuffer(GL_ARRAY_BUFFER, m_cube_VAO_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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
		// transparent VAO
		glGenVertexArrays(1, &m_transparent_VAO_);
		glGenBuffers(1, &m_transparent_VBO_);
		glBindVertexArray(m_transparent_VAO_);
		glBindBuffer(GL_ARRAY_BUFFER, m_transparent_VBO_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		// load textures
		// -------------
		m_cube_texture_ = TextureLoader::LoadTexture("assets/texture/dalishi.jpg", ETextureType::none).id_;
		m_floor_texture_ = TextureLoader::LoadTexture("assets/texture/jinshu.jpg", ETextureType::none).id_;
		m_transparent_grass_ = TextureLoader::LoadTexture("assets/texture/grass.png", ETextureType::none).id_;
		m_transparent_window_ = TextureLoader::LoadTexture("assets/texture/blending_transparent_window.png", ETextureType::none).id_;

		ShaderLoader shader_loader;

		m_shader_ = shader_loader.Load(R"(assets/shader/vs_texture.glsl)", R"(assets/shader/fs_blending.glsl)");
		m_shader_.Use();
		m_shader_.SetInt("texture1", 0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
		glBlendEquation(GL_FUNC_ADD);
	}

	void BlendingCase::DeInit()
	{
		glDeleteVertexArrays(1, &m_cube_VAO_);
		glDeleteVertexArrays(1, &m_plane_VAO_);
		glDeleteBuffers(1, &m_cube_VBO_);
		glDeleteBuffers(1, &m_plane_VBO_);
	}

}