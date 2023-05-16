#pragma once

#ifndef __SCENE
#define __SCENE

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "SkyBox.h"
#include "Screen.h"
#include "Light.h"
#include "Shapes/Plane.h"

#include <vector>

enum class EMouseState
{
	None = 0,
	Pressed_Left = 1,
	Released_Left = 2,
	Pressed_Right = 3,
	Released_Right = 4
};

class Scene
{
public:
	Scene();
	~Scene();

	void Render();

	void CheckMouseState();

	void HandleMouseMove();

	void HandleMouseRightPressed();

	void CheckKeyboardState();

	void Release();

	void AddMesh(Mesh* NewMesh);

	int ShouldWindowClose();

	Vector2f GetMousePos();

private:
	int WindowInit();

	void GenFrameBuffer(Vector2f ViewportSize); 
	
	void GenDepthFrameBuffer(Vector2f ViewportSize);

	void GetCameraInfo(Matrixf& OutView, Matrixf& OutProjection) const;
private:

	Camera* MainCamera = nullptr;
	std::vector<Mesh*> Meshes;

	bool bRenderDataDirty = true;

	GLFWwindow* Window = nullptr;

	Vector2f LastMousePos;
	EMouseState MouseState;

	SkyBox* Sky = nullptr;
	Screen* Viewport = nullptr;
	MeshRenderer* MeshRender = nullptr;
	Light* DirLight = nullptr;

	unsigned int FBO, ColorTexture, RBO;
	unsigned int DepthFBO, DepthTexture;
};

#endif



