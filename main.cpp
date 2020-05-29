#include <iostream>
#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "shaderLoader.h"
#include "stb/stb_image.h"
#include "Logger.h"
#include <random>

//#debug begin
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
//#debug end

#if 1

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

struct stTextureInfo
{
	int width = 0;
	int height = 0;
	int nrChannels = 0;
	unsigned char* ptrData = nullptr;
	stTextureInfo() = default;
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW-Window", NULL, NULL);
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

	int nAttriLimit;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttriLimit);

	ShaderLoader shader_interpolate(R"(shader/vs.glsl)", R"(shader/fs_texture.glsl)");
	ShaderLoader shader_uniform(R"(shader/vs.glsl)", R"(shader/fs_texture.glsl)");

	float vertices[] = {
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

	// glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
	// @index Specifies the index of the generic vertex attribute to be modified.
	// @size Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4
	// @type Specifies the data type of each component in the array
	// @stride Specifies the byte offset between consecutive generic vertex attributes. 
	//	If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0
	// @ pointer Specifies a offset of the first component of the first generic vertex attribute in the array 
	//	in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	//glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(float), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	//glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(2);

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
	const std::string texture_fabric_path{"texture/fabric.jpg"};
	const std::string texture_wood_path{"texture/wood.jpg"};
	const std::string texture_cartoon_path{"texture/cartoon.jpg"};

	stTextureInfo textures_data[2]{};
	textures_data[0].ptrData = stbi_load(texture_wood_path.c_str(), &textures_data[0].width, &textures_data[0].height, &textures_data[0].nrChannels, 0);
	textures_data[1].ptrData = stbi_load(texture_cartoon_path.c_str(), &textures_data[1].width, &textures_data[1].height, &textures_data[1].nrChannels, 0);
	//textures_data[1].ptrData = stbi_load(texture_fabric_path.c_str(), &textures_data[1].width, &textures_data[1].height, &textures_data[1].nrChannels, 0);

	unsigned int textures[2];
	glGenTextures(2, textures);
	for(size_t i = 0; i < sizeof(textures_data)/sizeof(textures_data[0]); i++)
	{	
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		const auto ptr_data = textures_data[i].ptrData;
		if (ptr_data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textures_data[i].width, textures_data[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, textures_data[i].ptrData);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(textures_data[i].ptrData);
			textures_data[i].ptrData = nullptr;
		}
		else
		{
			Logger::Error("Failed to load texture. Index = " + std::to_string(i));
		}
	}

	shader_interpolate.Use();
	shader_interpolate.SetInt("texture0", 0);
	shader_interpolate.SetInt("texture1", 1);

	shader_uniform.Use();
	shader_uniform.SetInt("texture0", 0);
	shader_uniform.SetInt("texture1", 1);
	
	glm::vec4 color_a{ 1.0f, 1.0f, 0.0f, 1.0f };

	glm::vec4 color_b{ 0.0f, 1.0f, 1.0f, 1.0f };

	float t = 1.0f;

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

	glm::vec3 arr_positon [12]{};
	for (size_t i = 0; i < 12; i++)
	{
		arr_positon[i] = fRand(-500, 500);
		arr_positon[i] /= 50.f;
	}

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

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, textures[1]);

			float timeValue = glfwGetTime();
			const float normalized_value = (sin(timeValue) / 2.0f) + 0.5f;
			shader_uniform.Use();
			shader_uniform.SetFloatVec("ourColor", glm::vec4{normalized_value, 0.0f, 0.0f, 1.0f});

			glBindVertexArray(VAO[1]);


			// transform
			for(size_t i = 0; i < 12; i++)
			{
				glm::vec3 axis{ 0.57735f, 0.57735f, 0.57735f };
				const float Sin = sin(timeValue * 0.5f + 20.0f * i);
				glm::quat q{ cos(timeValue * 0.5f), axis.x * Sin, axis.y * Sin, axis.z * Sin };
				q = glm::normalize(q);
				auto modelMatrix = glm::mat4{ q };
				const glm::vec3& position = arr_positon[i];
				modelMatrix = glm::translate(modelMatrix, position);

				auto viewMatrix = glm::translate(glm::mat4{ 1, }, glm::vec3{ 0.0f, 0.0f, -30.0f });
				auto projectMatrix = glm::perspective(glm::radians(30.f), 800.f / 600.f, 0.1f, 1000.f);
				shader_uniform.SetMatrix("model", modelMatrix);
				shader_uniform.SetMatrix("view", viewMatrix);
				shader_uniform.SetMatrix("projection", projectMatrix);
				
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		}

		glfwPollEvents();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

#endif