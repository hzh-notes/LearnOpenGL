#include "Scene.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Quat.h"
#include "Macro.h"

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
	ShaderProgramMap::GetInstance()->AddShaderProgram(0, "\\Shader\\VertexShader.glsl", "\\Shader\\PixelShader.glsl");
	MainCamera = new Camera(Transform(Vector3f(-150.f, 0.f, 0.f), Vector3f(0.f, 0.f, 0.f)));
	Sky = new SkyBox();
}

void Scene::Render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//更新鼠标位置
	glfwSetCursorPosCallback(Window, [](GLFWwindow* window, double xpos, double ypos) 
		{
			glfwSetCursorPos(window, xpos, ypos); 
		}
	);
	
	CheckMouseState();

	CheckKeyboardState();

	if (bRenderDataDirty)
	{
		Matrix model, view, projection;
		GetCameraInfo(view, projection);

		Sky->Render(view, projection);

		ShaderProgram* Program = ShaderProgramMap::GetInstance()->GetByKey(0);

		if (Program)
		{
			Program->Use();

			Program->SetUniform4x4("view", view);
			Program->SetUniform4x4("projection", projection);
			Program->SetUniform3f("viewPos", MainCamera->transform.Position);

			//light
			Program->SetUniform3f("spotLight.position", Vector3f(0.f, 0.f, 100.f));
			Program->SetUniform3f("spotLight.direction", Vector3f(0.86, 0, -0.5));
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

void Scene::CheckMouseState()
{
	MouseState = EMouseState::None;

	int LeftMouse = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT);
	if (LeftMouse)
	{
		if (LeftMouse == GLFW_PRESS)
		{
			MouseState = EMouseState::Pressed_Left;
		}
		else
		{
			MouseState = EMouseState::Released_Left;
		}
	}
	
	int RightMouse = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT);
	if (RightMouse)
	{
		if (RightMouse == GLFW_PRESS)
		{
			MouseState = EMouseState::Pressed_Right;
		}
		else
		{
			MouseState = EMouseState::Released_Right;
		}
	}

	Vector2f CurMousePos = GetMousePos();

	if (LastMousePos != CurMousePos)
	{
		HandleMouseMove();
	}
	LastMousePos = CurMousePos;
}

void Scene::HandleMouseMove()
{
	if (MouseState == EMouseState::Pressed_Right)
	{
		Vector2f CurMousePos = GetMousePos();

		Quat4f PreQuat = MainCamera->transform.Rotation;

		float Pitch = (CurMousePos.Y - LastMousePos.Y) / 10.f;
		float Yaw = (CurMousePos.X - LastMousePos.X) / 20.f;

		Vector3f CurAxisY = MainCamera->transform.TransformVector4f(Vector3f(0.f, -1.f, 0.f));
		Quat4f QuatPitch = Quat4f(CurAxisY, DegreeToRadian(Pitch));
		Quat4f QuatYaw = Quat4f(Vector3f(0.f, 0.f, -1.f), DegreeToRadian(Yaw));

		MainCamera->transform.Rotation = PreQuat * QuatPitch * QuatYaw;
	}
}

void Scene::HandleMouseRightPressed()
{
	Vector2f MousePos = GetMousePos();
	
}

void Scene::CheckKeyboardState()
{
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(Window, true);
	}
	else if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Vector3f Forward = MainCamera->transform.TransformVector4f(Vector3f(1.f, 0.f, 0.f));

		MainCamera->transform.Position += Forward.Normalize() * 20.f;
	}
	else if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Vector3f Left = MainCamera->transform.TransformVector4f(Vector3f(0.f, -1.f, 0.f));

		MainCamera->transform.Position += Left.Normalize() * 20.f;
	}
	else if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Vector3f Back = MainCamera->transform.TransformVector4f(Vector3f(-1.f, 0.f, 0.f));

		MainCamera->transform.Position += Back.Normalize() * 20.f;
	}
	else if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Vector3f Right = MainCamera->transform.TransformVector4f(Vector3f(0.f, 1.f, 0.f));

		MainCamera->transform.Position += Right.Normalize() * 20.f;
	}
}

void Scene::Release()
{
	//释放资源
	delete MainCamera;
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

Vector2f Scene::GetMousePos()
{
	double x;
	double y;
	glfwGetCursorPos(Window, &x, &y);

	//std::cout << "x :" << x << ", y :" << y << std::endl;

	return Vector2f(x, y);
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
