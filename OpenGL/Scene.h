#pragma once

#ifndef __SCENE
#define __SCENE

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include <vector>
#include <map>

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
public:

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

	void GatherMeshInfo(Mesh* InMesh, std::vector<MeshVertex>& OutVertices, std::vector<int>& OutIndices, Matrix& OutModel);

private:

	Camera* MainCamera = nullptr;
	std::vector<Mesh*> Meshes;

	bool bRenderDataDirty = true;

	GLFWwindow* Window = nullptr;

	std::map<int, ShaderProgram*> Programs;

	Vector2f LastMousePos;
	EMouseState MouseState;

	
};

#endif



