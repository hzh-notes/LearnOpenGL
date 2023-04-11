#include "Scene.h"
#include "Math/Quat.h"
#include "Math/Macro.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Scene::Scene()
{
	WindowInit();
	//������ȼ��
	glEnable(GL_DEPTH_TEST);
	MainCamera = new Camera(Transform(Vector3f(-150.f, 0.f, 0.f), Vector3f(0.f, 0.f, 0.f)));
	Sky = new SkyBox();
	Viewport = new Screen();
	MeshRender = new MeshRenderer();
}

Scene::~Scene()
{
	Release();
}

void Scene::Render()
{
	//�������λ��
	glfwSetCursorPosCallback(Window, [](GLFWwindow* window, double xpos, double ypos) 
		{
			glfwSetCursorPos(window, xpos, ypos); 
		}
	);
	//�������
	CheckMouseState();
	//��������
	CheckKeyboardState();

	if (bRenderDataDirty)
	{
		Matrix model, view, projection;
		GetCameraInfo(view, projection);

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClearColor(1.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_CULL_FACE);

		//glDepthMask(GL_FALSE);
		//glDepthFunc(GL_LEQUAL);

		glDisable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);
		Sky->Render(view, projection);

		glEnable(GL_DEPTH_TEST);
		//glDepthMask(GL_TRUE);
		glFrontFace(GL_CCW);
		MeshRender->Render(Meshes, view, projection, MainCamera->transform.Position);

		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ColorTexture);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // ����Ĭ��
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Viewport->Render(ColorTexture);
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
	//�ͷ���Դ
	delete MainCamera, Sky, MeshRender, Viewport;
	MainCamera = nullptr;
	Sky = nullptr;
	MeshRender = nullptr;
	Viewport = nullptr;
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
	//��ֹ�޸Ĵ��ڴ�С
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//���ò�������������
	glfwWindowHint(GLFW_SAMPLES, 4);
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
	//���ö��ز���
	glEnable(GL_MULTISAMPLE);
	//�����ӿڴ�С
	glViewport(0, 0, 1080, 720);
	GenFrameBuffer(Vector2f(1080, 720));
	//ע�����ô��ڴ�С�Ļص�
	//glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return 0;
}

void Scene::GenFrameBuffer(Vector2f ViewportSize)
{
	//unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//��������
	//unsigned int ColorTexture;
	glGenTextures(1, &ColorTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ViewportSize.X, ViewportSize.Y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTexture, 0);

	//��Ⱦ������󸽼�
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ViewportSize.X, ViewportSize.Y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void Scene::GetCameraInfo(Matrix& OutView, Matrix& OutProjection) const
{
	OutView = MainCamera->GetViewMatrix();
	OutProjection = MainCamera->GetProjectMatrix();
}