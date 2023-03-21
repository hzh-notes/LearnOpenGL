#include "Scene.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <iostream>
#include <direct.h>
#include <windows.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Scene::Scene()
{
	WindowInit();
	//开启深度检测
	glEnable(GL_DEPTH_TEST);
	//编译着色器
	ShaderCompile();
	MainCamera = new Camera(Transform(Vector3f(-150.f, 0.f, 50.f), Vector3f(0.f, -10.f, 0.f)));
}

void Scene::Render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (bRenderDataDirty)
	{
		Matrix model, view, projection;
		GetCameraInfo(view, projection);

		if (Program)
		{
			Program->SetUniform4x4("view", view);
			Program->SetUniform4x4("projection", projection);
			Program->SetUniform3f("viewPos", MainCamera->transform.Position);

			//light
			Program->SetUniform3f("spotLight.position", Vector3f(0.f, 0.f, 0.f));
			Program->SetUniform3f("spotLight.direction", Vector3f(1, 0, 0));
			Program->SetUniform3f("spotLight.ambient", Vector3f(0.1f));
			Program->SetUniform3f("spotLight.diffuse", Vector3f(0.5f));
			Program->SetUniform3f("spotLight.specular", Vector3f(1.f));
			Program->SetUniform1f("spotLight.constant", 1.f);
			Program->SetUniform1f("spotLight.linear", 0.0014f);
			Program->SetUniform1f("spotLight.quadratic", 0.000007f);
			Program->SetUniform1f("spotLight.innerCutOff", cos(3.1415926f / 24.f));
			Program->SetUniform1f("spotLight.outerCutOff", cos(3.1415926f / 12.f));

			Program->SetUniform3f("dirLight.direction", Vector3f(0.f, -1.f, 0.f));
			Program->SetUniform3f("dirLight.ambient", Vector3f(0.1f));
			Program->SetUniform3f("dirLight.diffuse", Vector3f(0.5f));
			Program->SetUniform3f("dirLight.specular", Vector3f(1.f));

			for (Mesh* mesh : Meshes)
			{
				std::vector<MeshVertex> Vertices;
				std::vector<int> Indices;

				GatherMeshInfo(mesh, Vertices, Indices, model);

				VertexBuffer* vBuffer = new VertexBuffer(Vertices.data(), sizeof(MeshVertex) * Vertices.size());
				IndexBuffer* iBuffer = new IndexBuffer(Indices.data(), sizeof(int) * Indices.size());
				Material mat = mesh->GetMaterial();

				Program->SetUniform4x4("model", model);

				//material
				if (!mat.diffuse.empty())
				{
					Program->SetUniformTexture2D("material.diffuse", mat.diffuse, 0);
				}
				if (!mat.specular.empty())
				{
					Program->SetUniformTexture2D("material.specular", mat.specular, 1);
				}
				Program->SetUniform1i("material.bEmission", mat.bEmission);
				if (!mat.emission.empty())
				{
					Program->SetUniformTexture2D("material.emission", mat.emission, 2);
				}
				Program->SetUniform1i("material.shininess", mat.shininess);

				//绑定顶点和索引
				glBindVertexArray(vBuffer->BufferId);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer->BufferId);
				//绘制模式
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				//绘制
				//glDrawArrays(GL_TRIANGLES, 0, indices.size());
				glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
			}
		}
	}

	glfwPollEvents();
	glfwSwapBuffers(Window);
}

void Scene::Release()
{
	//释放资源
	delete Program, MainCamera;
	Program = nullptr;
	MainCamera = nullptr;
	glfwTerminate();
}

void Scene::AddMesh(Mesh* NewMesh)
{
	Meshes.push_back(NewMesh);
}

int Scene::ShouldWindowClose()
{
	return glfwWindowShouldClose(Window);
}

int Scene::WindowInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//初始化窗口
	Window = glfwCreateWindow(1080, 720, "LearnOpenGL", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Window);

	//加载glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//设置视口大小
	glViewport(0, 0, 1080, 720);
	//注册设置窗口大小的回调
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
	return 0;
}

void Scene::ShaderCompile()
{
	char buffer[MAX_PATH];
	_getcwd(buffer, MAX_PATH);
	std::string path = buffer;
	std::string vspath = path + "\\Shader\\VertexShader.glsl";
	std::string pspath = path + "\\Shader\\PixelShader.glsl";

	Shader* VertexShader = new Shader(vspath.c_str(), EShaderType::VertexShader);
	Shader* PixelShader = new Shader(pspath.c_str(), EShaderType::PixelShader);

	if (VertexShader->Compile() && PixelShader->Compile())
	{
		Program = new ShaderProgram(VertexShader->ShaderId, PixelShader->ShaderId);

		if (Program->Compile())
		{
			Program->Use();
		}
		else
		{
			delete Program;
			Program = nullptr;
		}

		glDeleteShader(VertexShader->ShaderId);
		glDeleteShader(PixelShader->ShaderId);
	}
	else
	{
		return;
	}

	delete VertexShader, PixelShader;
	VertexShader = nullptr;
	PixelShader = nullptr;
}

void Scene::GetCameraInfo(Matrix& OutView, Matrix& OutProjection) const
{
	OutView = MainCamera->GetViewMatrix();
	OutProjection = MainCamera->GetProjectMatrix();
}

void Scene::GatherMeshInfo(Mesh* InMesh, std::vector<MeshVertex>& OutVertices, std::vector<int>& OutIndices, Matrix& OutModel)
{
	OutModel = InMesh->MeshTransform.GetMatrixWithScale();
	
	//Matrix mvpMatrix = modelMatrix * viewMatrix * projectMatrix;

	/*Vector3f point(50, 0, 0);
	Vector3f v1 = modelMatrix.TransformPosition(point);
	Vector3f v2 = viewMatrix.TransformPosition(v1);
	Vector3f v3 = projectMatrix.TransformPosition(v2);*/

	InMesh->GetElementInfo(OutVertices, OutIndices);
}
