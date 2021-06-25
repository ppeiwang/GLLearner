#include "testCases/OffScreenRenderCase.h"
#include "Config.h"
#include "glad/glad.h"
#include "tools/shaderLoader.h"
#include "tools/TextureLoader.h"
#include "scene/Scene.h"
#include "camera/Camera.h"

using namespace RenderCore;

namespace Test
{
void OffScreenRenderCase::Update()
{
	m_shader_.Use();

	std::shared_ptr<Camera> ptr_camera = m_scene_instance_->GetCamera();

	Camera& camera_instance_ = *ptr_camera;

	m_shader_.SetMatrix("projection", camera_instance_.GetProjectMatrix());
	m_shader_.SetMatrix("view", camera_instance_.GetViewMatrix());

	DrawMesh();
}

void OffScreenRenderCase::DrawMesh()
{
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
	glBindTexture(GL_TEXTURE_2D, m_plane_texture_);
	model = glm::mat4(1.0f);
	m_shader_.SetMatrix("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void OffScreenRenderCase::Init()
{
	float cubeVertices[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};
	float planeVertices[] = {
		// positions          // texture Coords
		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
		5.0f, -0.5f, -5.0f, 2.0f, 2.0f
	};

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates. NOTE that this plane is now much smaller and at the top of the screen
							 // positions   // texCoords
							 -0.3f, 1.0f, 0.0f, 1.0f,
							 -0.3f, 0.7f, 0.0f, 0.0f,
							 0.3f, 0.7f, 1.0f, 0.0f,

							 -0.3f, 1.0f, 0.0f, 1.0f,
							 0.3f, 0.7f, 1.0f, 0.0f,
							 0.3f, 1.0f, 1.0f, 1.0f
	};

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

	// screen quad VAO
	glGenVertexArrays(1, &m_quad_VAO_);
	glGenBuffers(1, &m_quad_VBO_);
	glBindVertexArray(m_quad_VAO_);
	glBindBuffer(GL_ARRAY_BUFFER, m_quad_VBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// load textures
	// -------------
	m_cube_texture_ = TextureLoader::LoadTexture("assets/texture/container.jpg", ETextureType::none).id_;
	m_plane_texture_ = TextureLoader::LoadTexture("assets/texture/fabric.jpg", ETextureType::none).id_;

	ShaderLoader shader_loader;

	m_shader_ = shader_loader.Load(R"(assets/shader/vs_texture.glsl)", R"(assets/shader/fs_blending.glsl)");
	m_shader_.Use();
	m_shader_.SetInt("texture1", 0);

	// bind the framebuffer to a texture buffer
	{
		unsigned int framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// create a color attachment texture
		unsigned int textureColorbuffer;
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, global::k_window_width, global::k_window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, global::k_window_width, global::k_window_height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);				  // now actually attach it
																													  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::Error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void OffScreenRenderCase::DeInit()
{
	glDeleteVertexArrays(1, &m_cube_VAO_);
	glDeleteBuffers(1, &m_cube_VBO_);
}

}