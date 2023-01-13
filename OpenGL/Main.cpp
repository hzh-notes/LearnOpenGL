#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Vector.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "stb_image.h"
#include "Matrix.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
GLenum DrawMode = GL_FILL;
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		DrawMode = GL_LINE;
	}
}


int LoadImage(std::string ImagPath, int index, unsigned int& OutTexture)
{
	glGenTextures(1, &OutTexture);

	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, OutTexture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int Width, Height, NumChannels;
	unsigned char* Data = stbi_load(ImagPath.c_str(), &Width, &Height, &NumChannels, 0);
	if (Data)
	{
		GLint TextureFormat = GL_RGB;
		if (NumChannels == 4)
		{
			TextureFormat = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, TextureFormat, Width, Height, 0, TextureFormat, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return -1;
	}

	stbi_image_free(Data);
	return 1;
}

int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//初始化窗口
	GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//加载glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//设置视口大小
	glViewport(0, 0, 800, 800);
	//注册设置窗口大小的回调
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	
	VertexDescription vertices[] = {
		VertexDescription(Vector3f(0.5f,  0.5f, 0.0f),Vector4f(1.0f, 0.0f, 0.0f, 1.0f), Vector2f(1.0f, 1.0f)),
		VertexDescription(Vector3f(0.5f, -0.5f, 0.0f),Vector4f(0.0f, 1.0f, 0.0f, 1.0f), Vector2f(1.0f, 0.0f)),
		VertexDescription(Vector3f(-0.5f, -0.5f, 0.0f),Vector4f(0.0f, 0.0f, 1.0f, 1.0f), Vector2f(0.0f, 0.0f)),
		VertexDescription(Vector3f(-0.5f,  0.5f, 0.0f),Vector4f(1.0f, 1.0f, 0.0f, 1.0f), Vector2f(0.0f, 1.0f)),
	};

	//cout << vertices << endl;

	VertexBuffer* vBuffer = new VertexBuffer(vertices, sizeof(vertices));

	unsigned int indices[] = {
		// 注意索引从0开始! 
		// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
		// 这样可以由下标代表顶点组合成矩形

		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	IndexBuffer* iBuffer = new IndexBuffer(indices, sizeof(indices));

#pragma region ShaderCompile

	unsigned int vertexShader;
	unsigned int fragmentShader;
	ShaderProgram* Program = nullptr;

	Shader* VertexShader = new Shader("E:/C++/LearnOpenGL/Shader/VertexShader.txt", EShaderType::VertexShader);
	Shader* PixelShader = new Shader("E:/C++/LearnOpenGL/Shader/PixelShader.txt", EShaderType::PixelShader);
	

	if (VertexShader->Compile() && PixelShader->Compile())
	{
		vertexShader = VertexShader->ShaderId;
		fragmentShader = PixelShader->ShaderId;

		Program = new ShaderProgram(vertexShader, fragmentShader);
		if (Program->Compile())
		{
			Program->Use();
		}
		else
		{
			delete Program;
			Program = nullptr;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	else
	{
		return -1;
	}

#pragma endregion
	unsigned int texture0;
	LoadImage("E:/container.jpg", 0, texture0);

	unsigned int texture1;
	LoadImage("E:/awesomeface.png",1, texture1);
	/*glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);*/

	Program->SetUniform1i("texture0", 0);
	Program->SetUniform1i("texture1", 1);

	//绑定顶点和索引
	glBindVertexArray(vBuffer->BufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer->BufferId);

	int NumIndices = iBuffer->GetNumIndices();

	//渲染
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

#pragma region Rendering

		// 激活着色器
		// 更新uniform颜色
		/*if (Program)
		{
			float timeValue = glfwGetTime();
			float greenValue = sin(timeValue) / 2.0f + 0.5f;
			Program->SetUniform4F("ColorTest", Vector4f(1.0f, greenValue, 1.0f, 1.0f));
			
		}*/
		
		
		glPolygonMode(GL_FRONT_AND_BACK, DrawMode);

		
		//绘制
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);
#pragma endregion

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//释放资源
	glfwTerminate();
	return 0;
}