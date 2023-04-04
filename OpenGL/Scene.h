#pragma once

#ifndef __SCENE
#define __SCENE

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "SkyBox.h"

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

	void GetCameraInfo(Matrix& OutView, Matrix& OutProjection) const;
private:

	Camera* MainCamera = nullptr;
	std::vector<Mesh*> Meshes;

	bool bRenderDataDirty = true;

	GLFWwindow* Window = nullptr;

	Vector2f LastMousePos;
	EMouseState MouseState;

	SkyBox* Sky = nullptr;

	MeshRenderer* MeshRender = nullptr;
};

#endif



