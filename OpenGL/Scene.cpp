#include "Scene.h"
#include "Quat.h"
#include "Macro.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Scene::Scene()
{
	WindowInit();
	//开启深度检测
	glEnable(GL_DEPTH_TEST);
	MainCamera = new Camera(Transform(Vector3f(-150.f, 0.f, 0.f), Vector3f(0.f, 0.f, 0.f)));
	Sky = new SkyBox();
	MeshRender = new MeshRenderer();
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

		MeshRender->Render(Meshes, view, projection, MainCamera->transform.Position);
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
	if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Vector3f Forward = MainCamera->transform.TransformVector4f(Vector3f(1.f, 0.f, 0.f));

		MainCamera->transform.Position += Forward.Normalize() * 20.f;
	}
	if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Vector3f Left = MainCamera->transform.TransformVector4f(Vector3f(0.f, -1.f, 0.f));

		MainCamera->transform.Position += Left.Normalize() * 20.f;
	}
	if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Vector3f Back = MainCamera->transform.TransformVector4f(Vector3f(-1.f, 0.f, 0.f));

		MainCamera->transform.Position += Back.Normalize() * 20.f;
	}
	if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Vector3f Right = MainCamera->transform.TransformVector4f(Vector3f(0.f, 1.f, 0.f));

		MainCamera->transform.Position += Right.Normalize() * 20.f;
	}
}

void Scene::Release()
{
	//释放资源
	delete MainCamera, Sky, MeshRender;
	MainCamera = nullptr;
	Sky = nullptr;
	MeshRender = nullptr;
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
