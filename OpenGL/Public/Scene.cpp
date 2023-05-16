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
	//开启深度检测
	glEnable(GL_DEPTH_TEST);
	MainCamera = new Camera(Transform(Vector3f(-150.f, 0.f, 0.f), Vector3f(0.f, 0.f, 0.f)));
	Sky = new SkyBox();
	Viewport = new Screen();
	MeshRender = new MeshRenderer();
	DirLight = new Light(Transform(Vector3f(-100, 0, 100), Vector3f(0, 30, 0)));
}

Scene::~Scene()
{
	Release();
}

void Scene::Render()
{
	//更新鼠标位置
	glfwSetCursorPosCallback(Window, [](GLFWwindow* window, double xpos, double ypos)
		{
			glfwSetCursorPos(window, xpos, ypos);
		}
	);
	//鼠标输入
	CheckMouseState();
	//键盘输入
	CheckKeyboardState();

	if (bRenderDataDirty)
	{
		Matrixf model, view, projection;
		GetCameraInfo(view, projection);

		//渲染深度图
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, DepthFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		DirLight->Render(Meshes);
		glCullFace(GL_BACK);

		//离屏渲染
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);

		/*glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);*/

		//天空球
		glDisable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);
		Sky->Render(view, projection);

		//渲染Mesh
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CCW);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, DepthTexture);
		MeshRender->Render(Meshes, view, projection, MainCamera->transform.Position, DirLight->GetLightSpaceMatrix());

		//渲染至视口
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // 返回默认
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ColorTexture);
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Viewport->Render();
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
		Quat4f QuatPitch = Quat4f(CurAxisY, DegToRad * Pitch);
		Quat4f QuatYaw = Quat4f(Vector3f(0.f, 0.f, -1.f), DegToRad * Yaw);

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

		MainCamera->transform.Position += Forward.Normalize() * 10.f;
	}
	if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Vector3f Left = MainCamera->transform.TransformVector4f(Vector3f(0.f, -1.f, 0.f));

		MainCamera->transform.Position += Left.Normalize() * 10.f;
	}
	if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Vector3f Back = MainCamera->transform.TransformVector4f(Vector3f(-1.f, 0.f, 0.f));

		MainCamera->transform.Position += Back.Normalize() * 10.f;
	}
	if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Vector3f Right = MainCamera->transform.TransformVector4f(Vector3f(0.f, 1.f, 0.f));

		MainCamera->transform.Position += Right.Normalize() * 10.f;
	}
}

void Scene::Release()
{
	//释放资源
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
	//禁止修改窗口大小
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//设置采样缓冲样本数
	//glfwWindowHint(GLFW_SAMPLES, 4);
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
	//启用多重采样
	//glEnable(GL_MULTISAMPLE);
	//设置视口大小
	glViewport(0, 0, 1080, 720);
	GenFrameBuffer(Vector2f(1080, 720));
	GenDepthFrameBuffer(Vector2f(1080, 720));
	//注册设置窗口大小的回调
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

	//纹理附件
	//unsigned int ColorTexture;
	glGenTextures(1, &ColorTexture);
	glBindTexture(GL_TEXTURE_2D, ColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ViewportSize.X, ViewportSize.Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTexture, 0);

	/*glGenTextures(1, &StencilTexture);
	glBindTexture(GL_TEXTURE_2D, StencilTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, ViewportSize.X, ViewportSize.Y, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, StencilTexture, 0);*/

	//渲染缓冲对象附件
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

void Scene::GenDepthFrameBuffer(Vector2f ViewportSize)
{
	glGenFramebuffers(1, &DepthFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, DepthFBO);

	glGenTextures(1, &DepthTexture);
	glBindTexture(GL_TEXTURE_2D, DepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ViewportSize.X, ViewportSize.Y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::GetCameraInfo(Matrixf& OutView, Matrixf& OutProjection) const
{
	OutView = MainCamera->GetViewMatrix();
	OutProjection = MainCamera->GetPerspectiveMatrix();
}
