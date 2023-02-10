#pragma once

#ifndef __SCENE
#define __SCENE

#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include <vector>

struct GLFWwindow;

class Scene
{
public:
	Scene();
public:

	void Render();

	void Release();

	void AddMesh(Mesh* NewMesh);

	int ShouldWindowClose();

private:
	int WindowInit();

	void ShaderCompile();

	void GatherMeshInfo(Mesh* InMesh, Matrix& OutModel, Matrix& OutView, Matrix& OutProjection);

private:

	Camera* mainCamera = nullptr;
	std::vector<Mesh*> Meshes;

	std::vector<VertexDescription> vertices;
	std::vector<int> indices;

	bool bRenderDataDirty = true;

	GLFWwindow* window = nullptr;
	ShaderProgram* Program = nullptr;
};

#endif



