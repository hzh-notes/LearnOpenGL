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

	void GetCameraInfo(Matrix& OutView, Matrix& OutProjection) const;

	void GatherMeshInfo(Mesh* InMesh, Matrix& OutModel);

private:

	Camera* MainCamera = nullptr;
	std::vector<Mesh*> Meshes;

	std::vector<MeshVertex> Vertices;
	std::vector<int> Indices;

	bool bRenderDataDirty = true;

	GLFWwindow* Window = nullptr;
	ShaderProgram* Program = nullptr;
};

#endif



