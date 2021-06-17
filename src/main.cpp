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
#include "light/Light.h"
#include "scene/Model.h"
#include <filesystem>
#include "testCases/BlendingCase.h"
#include "testCases/OffScreenRenderCase.h"

//#include "assimp/Importer.hpp"

//#debug begin
#include "math/PMath.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtc/matrix_transform.hpp"
//#debug end

//#define RENDER_CORE_GUI_PANEL

using namespace RenderCore;

Scene global_scene_instance{ "Simple Scene" };

void processInput(GLFWwindow* window);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	std::shared_ptr<Camera> ptr_camera = global_scene_instance.GetCamera();
	if (ptr_camera) {
		const float fov = ptr_camera->GetFov();
		const float near = ptr_camera->GetNear();
		const float far = ptr_camera->GetFar();
		ptr_camera->SetPerspective(fov, width / height, near, far);
	}
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
	// always initialize the logger at the first time
	{
		auto ptr_gui_logger = std::make_shared<GuiLogger>();
		Logger::GetInstance().SetGuiLogger(ptr_gui_logger);
#ifdef RENDER_CORE_GUI_PANEL
		GUIManager::GetInstance().AddGuiPanel(ptr_gui_logger);
#endif
	}

	glfwInit();

	/*
		Require a minimum OpenGL version by setting the GLFW_CONTEXT_VERSION_MAJOR and
		GLFW_CONTEXT_VERSION_MINOR hints before creation.
		If the required minimum version is not supported on the machine, context (and window) creation fails
	*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW-Window", NULL, NULL);
	//GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW-Window", glfwGetPrimaryMonitor(), NULL); // full screen

	{
		int left, top, right, bottom;
		glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);
		int debug = 1;
	}

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
		glfwTerminate();
		return -1;
	}

#ifdef RENDER_CORE_GUI_PANEL
	GUIManager::GetInstance().Init(window);
#endif

	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string

	const std::string title = std::string((char*)renderer) + ". OpenGL Version: " + std::string((char*)version);

	glfwSetWindowTitle(window, title.c_str());

	glViewport(0, 0, 800, 600);

	//  compile vertex shader and fragment shader

	int nAttriLimit;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttriLimit);

	glEnable(GL_DEPTH_TEST);

	//stbi_set_flip_vertically_on_load(true);

	Test::TestCase* current_case = new Test::OffScreenRenderCase();

	if (current_case)
	{
		current_case->SetScene(&global_scene_instance);
		current_case->Init();
	}


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	global_scene_instance.CreateCamera();

	global_scene_instance.SetLightPosition({ 1.2f, 1.0f, 2.0f });

	std::shared_ptr<Camera> ptr_camera = global_scene_instance.GetCamera();
	Camera& camera_instance_ = *ptr_camera;

	camera_instance_.SetPosition({ 0.0f, 0.0f, 10.0f });

	std::shared_ptr<CameraPanel> ptr_camera_debug_panel = std::make_shared<CameraPanel>();
	ptr_camera_debug_panel->SetCamera(ptr_camera);

#ifdef RENDER_CORE_GUI_PANEL
	GUIManager::GetInstance().AddGuiPanel(ptr_camera_debug_panel);
#endif

	unsigned long long frame = 0;
	unsigned long long ms = 0;

	constexpr std::chrono::seconds time_seconds{ 1 };
	constexpr std::chrono::nanoseconds time_per_frame_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time_seconds) / 60;
	constexpr std::chrono::milliseconds time_per_frame_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_per_frame_nanoseconds);

	std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();


	while (!glfwWindowShouldClose(window))
	{
		++frame;

		processInput(window);
		// rendering
		{
			glClearColor(0.15f, 0.15f, 0.18f, 1.0f);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			if (current_case)
			{
				current_case->Update();
			}

			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

#ifdef RENDER_CORE_GUI_PANEL
		GUIManager::GetInstance().Update();
#endif

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

	if (current_case)
	{
		current_case->DeInit();
	}

#ifdef RENDER_CORE_GUI_PANEL
	GUIManager::GetInstance().DeInit();
#endif

	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	glm::vec3 translation_increment{ 0 };
	glm::vec3 rotation_increment{ 0 };

	const float translation_delta = 0.2f;
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

	auto ptr_camera = global_scene_instance.GetCamera();
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

	const float asp = camera_instance_.GetAsp();
	const float near = camera_instance_.GetNear();
	const float far = camera_instance_.GetFar();
	//camera_instance_.SetPerspective(glm::radians(gZoom), asp, near, far);
	camera_instance_.SetPerspective(glm::radians(gZoom), 800.f / 600.f, 0.1f, 1000.f);


	camera_instance_.Update();
}
