#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "tools/shaderLoader.h"
#include "tools/TextureLoader.h"
#include "stb/stb_image.h"
#include "logger/Logger.h"
#include <random>
#include "camera/Camera.h"
#include "scene/Scene.h"
#include "gui/GUIManager.h"
#include "gui/GuiPanel.h"

//#debug begin
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtc/matrix_transform.hpp"
//#debug end

Scene global_scene_instance{"Simple Scene"};

void processInput(GLFWwindow* window);


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

struct stTextureInfo
{
	int width = 0;
	int height = 0;
	int nrChannels = 0;
	unsigned char* ptrData = nullptr;
	stTextureInfo() = default;
};


void SetWindowTitle(GLFWwindow* window, const std::string& title, float frame_rate)
{
	if (window)
	{
		std::stringstream ss;

		ss << std::fixed << std::setprecision(2) << frame_rate;
		std::string title_concat{ title };
		title_concat.append("   Frame Rate: ");
		title_concat.append(ss.str());
		glfwSetWindowTitle(window, title_concat.c_str());
	}

}

void mouse_callback(GLFWwindow* /*window*/, double /*xpos*/, double /*ypos*/)
{

}

static float gZoom = 45.0f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	gZoom -= (float)yoffset;
	if (gZoom < 1.0f)
		gZoom = 1.0f;
	if (gZoom > 45.0f)
		gZoom = 45.0f;
}

int main()
{
	auto q = glm::quat{0.731f, 0.4656f, 0.0f, 0.0f };

	float x, y, z;
	glm::extractEulerAngleXYZ(glm::mat4{q}, x, y, z);
	q = glm::quat{ -0.731f, 0.4656f, 0.0f, 0.0f };	
	float x1, y1, z1;

	glm::extractEulerAngleXYZ(glm::mat4{ q }, x1, y1, z1);


	auto m0 = glm::eulerAngleXYZ(0.f, glm::pi<float>(), 0.f);
	auto m1 = glm::eulerAngleXYZ(50.f / 180.f, 30.f / 180.f, 0.f);
	auto m2 = m1 * m0;
	glm::extractEulerAngleXYZ(m2, x, y, z);


	auto ptr_gui_logger = std::make_shared<GuiLogger>();
	Logger::GetInstance().SetGuiLogger(ptr_gui_logger);
	GUIManager::GetInstance().AddGuiPanel(ptr_gui_logger);

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW-Window", NULL, NULL);
	if (window == NULL)
	{
		Logger::Error("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Logger::Error("Failed to initialize GLAD");
		return -1;
	}

	GUIManager::GetInstance().Init(window);

	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string

	const std::string title = std::string((char*)renderer) + ". OpenGL Version: " + std::string((char*)version);

	glfwSetWindowTitle(window, title.c_str());

	glViewport(0, 0, 800, 600);

	//  compile vertex shader and fragment shader

	int nAttriLimit;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttriLimit);

	float vertices[] = {
			// positions          // normals          // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO[2], VAO[2], EBO[2];

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), then configure vertex attributes(s), and at last bind element buffer.
	
	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(float), indices, GL_STATIC_DRAW);

	/************************************************************************
	 glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);

	 @index Specifies the index of the generic vertex attribute to be modified.
	 @size Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4
	 @type Specifies the data type of each component in the array
	 @stride Specifies the byte offset between consecutive generic vertex attributes. 
		If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0
	 @ pointer Specifies a offset of the first component of the first generic vertex attribute in the array 
		in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(float), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	//glEnableVertexAttribArray(2);

	uint32_t light_VAO;
	glGenVertexArrays(1, &light_VAO);
	glBindVertexArray(light_VAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	// set the vertex attributes (only position data for our lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
	// attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but
	// this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we
	// generally don't unbind VAOs (nor VBOs) when it's not directly necessary.


	/*
		Usually when you have multiple objects you want to draw, you first generate/configure all the VAOs (and thus the required VBO and attribute pointers)
		and store those for later use. The moment we want to draw one of our objects, we take the corresponding VAO, bind it, 
		then draw the object and unbind the VAO again.	
	*/

	glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//const std::string texture_file_path{"blue-ice-background.jpg"};
	const std::string texture_fabric_path{"assets/texture/fabric.jpg"};
	const std::string texture_wood_path{"assets/texture/wood.jpg"};
	const std::string texture_cartoon_path{"assets/texture/cartoon.jpg"};
	const std::string texture_diffuse_path{"assets/texture/container2.png"};
	const std::string texture_specular_path{"assets/texture/container2_specular.png"};

	ShaderLoader shader_cube_on_light{ R"(assets/shader/vs_texture.glsl)", R"(assets/shader/fs_cube_on_light.glsl)" };
	ShaderLoader shader_light{ R"(assets/shader/vs_raw.glsl)", R"(assets/shader/fs_light.glsl)" };
	//ShaderLoader shader_interpolate(R"(shader/vs.glsl)", R"(shader/fs_texture.glsl)");

	shader_cube_on_light.Use();
	const auto texture_diffuse_id =  TextureLoader::LoadTexture(texture_diffuse_path.c_str());
	const auto texture_sepcular_id =  TextureLoader::LoadTexture(texture_specular_path.c_str());
	shader_cube_on_light.SetInt("material.diffuse", 0);
	shader_cube_on_light.SetInt("material.specular", 1);


	//shader_interpolate.Use();
	//shader_interpolate.SetInt("texture0", 1);
	//shader_interpolate.SetInt("texture1", 2);
	
	glm::vec4 color_a{ 1.0f, 1.0f, 0.0f, 1.0f };

	glm::vec4 color_b{ 0.0f, 1.0f, 1.0f, 1.0f };

	auto fRand = [](int min, int max)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(min, max);
		glm::vec3 v;
		for (size_t i = 0; i < 3; i++)
			v[i] = dis(gen);
		return v;
	};

	global_scene_instance.CreateCamera();

	global_scene_instance.SetLightPosition({ 1.2f, 1.0f, 2.0f });

	std::shared_ptr<Camera> ptr_camera = global_scene_instance.GetCamera();
	Camera& camera_instance_ = *ptr_camera;
	
	std::shared_ptr<CameraPanel> ptr_camera_debug_panel = std::make_shared<CameraPanel>();
	ptr_camera_debug_panel->SetCamera(ptr_camera);
	GUIManager::GetInstance().AddGuiPanel(ptr_camera_debug_panel);

	unsigned long long frame = 0;
	unsigned long long ms = 0;

	constexpr std::chrono::seconds time_seconds { 1 };
	constexpr std::chrono::nanoseconds time_per_frame_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time_seconds) / 60;
	constexpr std::chrono::milliseconds time_per_frame_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_per_frame_nanoseconds);

	std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window))
	{	
		++frame;

		processInput(window);

		// rendering
		{
			//glClearColor(color_a.x, color_a.y, color_a.z, color_a.w);
			glClearColor(0.15f, 0.15f, 0.18f, 1.0f);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_diffuse_id);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture_sepcular_id);

			auto modelMatrix = glm::mat4{ 1 };
			modelMatrix = glm::translate(modelMatrix, glm::vec3{ 0, 0, -5.f });

			// update light
			{
				const auto cube_pos = modelMatrix * glm::vec4{0.f,0.f,0.f, 1.f};
				float cur_time = glfwGetTime();
				const float radius = 3.0f;
				float light_x = cube_pos.x + glm::cos(cur_time) * radius;
				float light_y = cube_pos.y;
				float light_z = cube_pos.z + glm::sin(cur_time) * radius;

			//	global_scene_instance.SetLightPosition(light_x, light_y, light_z);
			}

			glm::vec3 lightColor{1.f, 1.f, 1.f};
			//lightColor.x = glm::abs(sin(glfwGetTime() * 2.0f));
			//lightColor.y = glm::abs(sin(glfwGetTime() * 0.7f));
			//lightColor.z = glm::abs(sin(glfwGetTime() * 1.3f));

			{
				shader_cube_on_light.Use();

				shader_cube_on_light.SetFloatVec("objectColor", { 1.0f, 0.5f, 0.31f });
				shader_cube_on_light.SetFloatVec("viewPos", camera_instance_.GetPosition());

				shader_cube_on_light.SetFloatVec("light.ambient", lightColor* glm::vec3{ 0.5f } *glm::vec3{0.2f});
				shader_cube_on_light.SetFloatVec("light.diffuse", lightColor* glm::vec3{ 0.5f }); // darken diffuse light a bit
				shader_cube_on_light.SetFloatVec("light.specular", 1.0f, 1.0f, 1.0f);
				shader_cube_on_light.SetFloatVec("light.position", global_scene_instance.GetLightPosition());

				shader_cube_on_light.SetFloat("material.shininess", 32.0f);

				const auto& viewMatrix = camera_instance_.GetViewMatrix();
				const auto& projMatrix = camera_instance_.GetProjectMatrix();

				shader_cube_on_light.SetMatrix("model", modelMatrix);
				shader_cube_on_light.SetMatrix("view", viewMatrix);
				shader_cube_on_light.SetMatrix("projection", projMatrix);
			
				glBindVertexArray(VAO[1]);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}	

			{
				shader_light.Use();
				auto modelMatrix = glm::mat4{ 1 };

				modelMatrix = glm::translate(modelMatrix, global_scene_instance.GetLightPosition());
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
				const auto& viewMatrix = camera_instance_.GetViewMatrix();
				const auto& projMatrix = camera_instance_.GetProjectMatrix();

				shader_light.SetFloatVec("uFragColor", lightColor.x, lightColor.y, lightColor.z, 1.0f);
				shader_light.SetMatrix("model", modelMatrix);
				shader_light.SetMatrix("view", viewMatrix);
				shader_light.SetMatrix("projection", projMatrix);

				glBindVertexArray(light_VAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

		
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		GUIManager::GetInstance().Update();

		glfwPollEvents();

		glfwSwapBuffers(window);

		// frame rate
		{
			const std::chrono::nanoseconds time_passed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - last);
			if (time_passed < time_per_frame_nanoseconds)
			{
				std::this_thread::sleep_for(time_per_frame_nanoseconds - time_passed);
			}

			const std::chrono::time_point<std::chrono::high_resolution_clock> time_stamp = std::chrono::high_resolution_clock::now();
			std::chrono::nanoseconds time_per_frame_nanosecond = time_stamp - last;
			const float time_per_frame_second = time_per_frame_nanoseconds.count() * 1e-9f;
			const float frame_rate = 1.0f / time_per_frame_second;
			last = time_stamp;

			SetWindowTitle(window, title, frame_rate);

			ms += time_per_frame_milliseconds.count();
		}

	}

	GUIManager::GetInstance().DeInit();

	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	glm::vec3 translation_increment{ 0 };
	glm::vec3 rotation_increment{ 0 };

	const float translation_delta = 0.5f;
	const float rotation_delta = 0.02f;
	// left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		translation_increment.x -= translation_delta;
	}

	// right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		translation_increment.x += translation_delta;
	}

	// forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		translation_increment.z = translation_delta;
	}

	// backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		translation_increment.z -= translation_delta;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		// x+
		rotation_increment.x += rotation_delta;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		// x-
		rotation_increment.x -= rotation_delta;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		// y+
		rotation_increment.y += rotation_delta;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		// y-
		rotation_increment.y -= rotation_delta;
	}

	auto ptr_camera =  global_scene_instance.GetCamera();
	auto& camera_instance_ = *ptr_camera;

	const auto right_direction = camera_instance_.GetRight();
	const auto up_direction = camera_instance_.GetUp();
	const auto forward_direction = camera_instance_.GetForward();

	const auto camera_current_position = camera_instance_.GetPosition();
	const auto camera_new_position = (camera_current_position)+
		right_direction * translation_increment.x +
		up_direction * translation_increment.y +
		forward_direction * translation_increment.z;

	camera_instance_.SetPosition(camera_new_position);

	camera_instance_.Pitch(rotation_increment.x);
	camera_instance_.Yaw(rotation_increment.y);
	camera_instance_.Roll(rotation_increment.z);
	camera_instance_.Update();

	camera_instance_.SetPerspective(glm::radians(gZoom), 800.f / 600.f, 0.1f, 1000.f);
}
