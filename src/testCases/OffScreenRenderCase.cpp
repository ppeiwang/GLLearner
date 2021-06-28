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
	std::shared_ptr<Camera> ptr_camera = m_scene_instance_->GetCamera();

	Camera& camera_instance_ = *ptr_camera;

	// first render pass: mirror texture.
	// bind to framebuffer and draw to color texture as we normally
	// would, but with the view camera reversed.
	// bind to framebuffer and draw scene as we normally would to color texture
	// ------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer_);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	// make sure we clear the framebuffer's content
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera_instance_.Rotate(glm::pi<float>(), glm::vec3{ 0.0f, 1.0f, 0.0f });
	camera_instance_.Update();

	m_shader_.Use();

	m_shader_.SetMatrix("projection", camera_instance_.GetProjectMatrix());
	m_shader_.SetMatrix("view", camera_instance_.GetViewMatrix());

	DrawMesh();

	// render pass II:

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera_instance_.Rotate(glm::pi<float>(), glm::vec3{ 0.0f, 1.0f, 0.0f }); // reset camera's direction
	camera_instance_.Update();

	m_shader_.SetMatrix("projection", camera_instance_.GetProjectMatrix());
	m_shader_.SetMatrix("view", camera_instance_.GetViewMatrix());

	DrawMesh();

	// now draw the mirror quad with screen texture
	// --------------------------------------------
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	m_screen_shader_.Use();
	glBindVertexArray(m_quad_VAO_);
	glBindTexture(GL_TEXTURE_2D, m_texture_color_buffer_); // use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
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

	m_screen_shader_ = shader_loader.Load(R"(assets/shader/vs_screen.glsl)", R"(assets/shader/fs_blending.glsl)");
	m_screen_shader_.Use();
	m_screen_shader_.SetInt("texture1", 0);

	// bind the framebuffer to a texture buffer
	{
		glGenFramebuffers(1, &m_frame_buffer_);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer_);
		// create a color attachment texture
		glGenTextures(1, &m_texture_color_buffer_);
		glBindTexture(GL_TEXTURE_2D, m_texture_color_buffer_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, global::k_window_width, global::k_window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_color_buffer_, 0);
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