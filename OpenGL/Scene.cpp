#include "Scene.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <iostream>
#include <direct.h>
#include <windows.h>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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

Scene::Scene()
{
	WindowInit();
	//开启深度检测
	glEnable(GL_DEPTH_TEST);
	//编译着色器
	ShaderCompile();
	mainCamera = new Camera();
}

void Scene::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (bRenderDataDirty)
	{
		for (Mesh* mesh : Meshes)
		{
			vertices.clear();
			indices.clear();

			Matrix model, view, projection;
			GatherMeshInfo(mesh, model, view, projection);

			VertexBuffer* vBuffer = new VertexBuffer(vertices.data(), sizeof(VertexDescription) * vertices.size());
			IndexBuffer* iBuffer = new IndexBuffer(indices.data(), sizeof(int) * indices.size());

			//设置uniform变量
			if (Program)
			{
				Program->SetUniform4x4("model", model);
				Program->SetUniform4x4("view", view);
				Program->SetUniform4x4("projection", projection);
			}

			//绑定顶点和索引
			glBindVertexArray(vBuffer->BufferId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer->BufferId);
			//绘制模式
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL/*GL_LINE*/);
			//绘制
			glDrawArrays(GL_TRIANGLES, 0, indices.size());
		}

		
		//bRenderDataDirty = false;
	}
	// 激活着色器
		// 更新uniform颜色
		/*if (Program)
		{
			float timeValue = glfwGetTime();
			float greenValue = sin(timeValue) / 2.0f + 0.5f;
			Program->SetUniform4F("ColorTest", Vector4f(1.0f, greenValue, 1.0f, 1.0f));

		}*/


	
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_INT, 0);

	glfwPollEvents();
	glfwSwapBuffers(window);
}

void Scene::Release()
{
	//释放资源
	glfwTerminate();
}

void Scene::AddMesh(Mesh* NewMesh)
{
	Meshes.push_back(NewMesh);
}

int Scene::ShouldWindowClose()
{
	return glfwWindowShouldClose(window);
}

int Scene::WindowInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//初始化窗口
	window = glfwCreateWindow(1080, 720, "LearnOpenGL", NULL, NULL);
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
	glViewport(0, 0, 1080, 720);
	//注册设置窗口大小的回调
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void Scene::ShaderCompile()
{
	unsigned int vertexShader;
	unsigned int fragmentShader;
	
	char buffer[MAX_PATH];
	_getcwd(buffer, MAX_PATH);
	std::string path = buffer;
	std::string vspath = path + "\\Shader\\VertexShader.txt";
	std::string pspath = path + "\\Shader\\PixelShader.txt";
	const char* aa = vspath.c_str();
	
	Shader* VertexShader = new Shader(vspath.c_str(), EShaderType::VertexShader);
	Shader* PixelShader = new Shader(pspath.c_str(), EShaderType::PixelShader);
	//Shader* VertexShader = new Shader("E:\\C++\\LearnOpenGL\\Shader\\VertexShader.txt", EShaderType::VertexShader);
	//Shader* PixelShader = new Shader("E:\\C++\\LearnOpenGL\\Shader\\PixelShader.txt", EShaderType::PixelShader);


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
		return;
	}

	unsigned int texture0;
	std::string texturepath0 = path + "\\Texture\\container.jpg";
	LoadImage(texturepath0, 0, texture0);

	unsigned int texture1;
	std::string texturepath1 = path + "\\Texture\\awesomeface.png";
	LoadImage(texturepath1, 1, texture1);

	Program->SetUniform1i("texture0", 0);
	Program->SetUniform1i("texture1", 1);
}

void Scene::GatherMeshInfo(Mesh* InMesh, Matrix& OutModel, Matrix& OutView, Matrix& OutProjection)
{
	OutModel = InMesh->transform.GetMatrixWithScale();
	OutView = mainCamera->GetViewMatrix();
	OutProjection = mainCamera->GetProjectMatrix();
	//Matrix mvpMatrix = modelMatrix * viewMatrix * projectMatrix;

	/*Vector3f point(50, 0, 0);
	Vector3f v1 = modelMatrix.TransformPosition(point);
	Vector3f v2 = viewMatrix.TransformPosition(v1);
	Vector3f v3 = projectMatrix.TransformPosition(v2);*/

	std::vector<Vector3f> positions = InMesh->positions;
	std::vector<Vector2f> uvs = InMesh->uvs;
	int index = indices.size();
	std::vector<int> meshIndices = InMesh->indices;

	for (int i = 0; i < meshIndices.size(); ++i)
	{
		int curIndex = meshIndices[i];
		/*Vector3f screenPosition = mvpMatrix.TransformPosition(positions[curIndex]);
		screenPosition.Z = 1.0 - screenPosition.Z;*/
		Vector2f uv = uvs[curIndex];
		vertices.push_back(VertexDescription(positions[curIndex], Vector4f(1), uv));
	}

	for (int i = 0; i < meshIndices.size(); ++i)
	{
		indices.push_back(meshIndices[i] + index);
	}

}
