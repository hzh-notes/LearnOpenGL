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

	void GatherMeshInfo(Mesh* InMesh, std::vector<MeshVertex>& OutVertices, std::vector<int>& OutIndices, Matrix& OutModel);

private:

	Camera* MainCamera = nullptr;
	std::vector<Mesh*> Meshes;

	bool bRenderDataDirty = true;

	GLFWwindow* Window = nullptr;
	ShaderProgram* Program = nullptr;
};

#endif



