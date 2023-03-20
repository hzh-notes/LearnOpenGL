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
	//������ȼ��
	glEnable(GL_DEPTH_TEST);
	//������ɫ��
	ShaderCompile();
	MainCamera = new Camera(Transform(Vector3f(-100.f, 0.f, 50.f), Vector3f(0.f, 0.f, 0.f)));
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
		for (Mesh* mesh : Meshes)
		{
			Vertices.clear();
			Indices.clear();

			GatherMeshInfo(mesh, model);

			VertexBuffer* vBuffer = new VertexBuffer(Vertices.data(), sizeof(MeshVertex) * Vertices.size());
			IndexBuffer* iBuffer = new IndexBuffer(Indices.data(), sizeof(int) * Indices.size());
			Material mat = mesh->GetMaterial();

			//����uniform����
			if (Program)
			{
				Program->SetUniform4x4("model", model);
				Program->SetUniform4x4("view", view);
				Program->SetUniform4x4("projection", projection);
				Program->SetUniform3f("viewPos", MainCamera->transform.Position);

				//light
				Program->SetUniform3f("light.lightDir", Vector3f(-0.2f, -1.f, -0.3f));
				Program->SetUniform3f("light.ambientStrength", Vector3f(0.1f));
				Program->SetUniform3f("light.diffuseStrength", Vector3f(0.5f));
				Program->SetUniform3f("light.specularStrength", Vector3f(1.f));

				//material
				Program->SetUniformTexture2D("material.diffuse", mat.diffuse/*"container2.png"*/, 0);
				Program->SetUniformTexture2D("material.specular", mat.specular/*"container2_specular.png"*/, 1);
				Program->SetUniform1i("material.bEmission", mat.bEmission);
				Program->SetUniformTexture2D("material.emission", mat.emission/*"matrix.jpg"*/, 2);
				Program->SetUniform1i("material.shininess", mat.shininess);
			}

			//�󶨶��������
			glBindVertexArray(vBuffer->BufferId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer->BufferId);
			//����ģʽ
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//����
			//glDrawArrays(GL_TRIANGLES, 0, indices.size());
			glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
		}

	}

	glfwPollEvents();
	glfwSwapBuffers(Window);
}

void Scene::Release()
{
	//�ͷ���Դ
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

	//��ʼ������
	Window = glfwCreateWindow(1080, 720, "LearnOpenGL", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Window);

	//����glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//�����ӿڴ�С
	glViewport(0, 0, 1080, 720);
	//ע�����ô��ڴ�С�Ļص�
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
}

void Scene::ShaderCompile()
{
	char buffer[MAX_PATH];
	_getcwd(buffer, MAX_PATH);
	std::string path = buffer;
	std::string vspath = path + "\\Shader\\VertexShader.txt";
	std::string pspath = path + "\\Shader\\PixelShader.txt";

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

void Scene::GatherMeshInfo(Mesh* InMesh, Matrix& OutModel)
{
	OutModel = InMesh->MeshTransform.GetMatrixWithScale();
	
	//Matrix mvpMatrix = modelMatrix * viewMatrix * projectMatrix;

	/*Vector3f point(50, 0, 0);
	Vector3f v1 = modelMatrix.TransformPosition(point);
	Vector3f v2 = viewMatrix.TransformPosition(v1);
	Vector3f v3 = projectMatrix.TransformPosition(v2);*/

	InMesh->GetElementInfo(Vertices, Indices);
}
