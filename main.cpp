#include <iostream>
#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
//#debug begin
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
//#debug end

int main()
{
	const auto half_pi = glm::pi<float>() / 2.0f;
	const auto fouth_pi = glm::pi<float>() / 4.0f;

	const auto angle = glm::vec3{ 1.23f, 0.21f, half_pi };

	const auto euler_z_matrix = glm::rotate(glm::mat4{ 1 }, angle.z, glm::vec3{0.f,0.f,1.0f});
	const auto euler_y_matrix = glm::rotate(glm::mat4{ 1 }, angle.y, glm::vec3{0.f,1.f,0.0f});
	const auto euler_x_matrix = glm::rotate(glm::mat4{ 1 }, angle.x, glm::vec3{1.f,0.f,0.0f});
	const auto euler_matrix = euler_z_matrix*euler_y_matrix*euler_x_matrix;

	const auto q = glm::quat{ angle };
	const auto q_matrix = glm::mat4{ q };

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
	{
			assert(glm::epsilonEqual(euler_matrix[i][j], q_matrix[i][j], 0.001f));
	}

	return 0;

}

#if 0

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

const char* vertexShaderSource = R"(#version 330 core
	layout (location = 0) in vec3 aPos;
	void main()
	{
	   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	})";

const char* fragmentShaderSource = R"(#version 330 core
			out vec4 FragColor;
			void main()
			{
				FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
			})";

const char* fragmentShaderSource1 = R"(#version 330 core
			out vec4 FragColor;
			void main()
			{
				FragColor = vec4(0.1f, 0.3f, 0.8f, 1.0f);
			})";

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

	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string

	const std::string title = std::string((char*)renderer) + ". OpenGL Version: " + std::string((char*)version);

	glfwSetWindowTitle(window, title.c_str());

	glViewport(0, 0, 800, 600);

	//  compile vertex shader and fregment shader

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::Link_FAILED\n" << infoLog << std::endl;
	}


	unsigned int fragmentShader1;
	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader1);
	unsigned int shaderProgram1;
	shaderProgram1 = glCreateProgram();

	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glLinkProgram(shaderProgram1);

	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::Link_FAILED\n" << infoLog << std::endl;
	}


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader1);

	float vertices[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.f,
		0.5f, 0.0f,  0.f,
		0.0f, 1.0f,  0.0f,
		-0.5f, 0.f,  0.0f
	};
	unsigned int indices[] = {
		1, 2, 0,
		0, 2, 3,
		3, 0, 4,
		4, 0, 1,
	};

	unsigned int VBO[2], VAO[2], EBO[2];

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(float), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(float), indices + 6, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
	// attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but
	// this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we
	// generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::vec4 color_a{ 1.0f, 1.0f, 0.0f, 1.0f };

	glm::vec4 color_b{ 0.0f, 1.0f, 1.0f, 1.0f };

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

			glUseProgram(shaderProgram);

			glBindVertexArray(VAO[0]);
			//glDrawArrays(GL_TRIANGLES, 0, 3);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


			glUseProgram(shaderProgram1);
			glBindVertexArray(VAO[1]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		}

		glfwPollEvents();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

#endif