#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "SJ", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glm::vec4 color_a{ 1.0f, 0.0f, 0.0f, 1.0f };

	glm::vec4 color_b{ 0.0f, 1.0f, 0.0f, 1.0f };

	float t = 1.0f;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// rendering
		{
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				if (t < 1.0f)
				{
					t += 0.0005f;
				}
			}

			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				if (t > 0.0f)
				{
					t -= 0.0005f;
				}
			}


			glm::vec4 color_c = (1 - t) * color_a + t * color_b;

			glClearColor(color_c.x, color_c.y, color_c.z, color_c.w);

			glClear(GL_COLOR_BUFFER_BIT);

		}


		glfwPollEvents();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}


