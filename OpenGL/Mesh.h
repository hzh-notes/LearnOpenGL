#pragma once
#include "Transform.h"
#include "VertexBuffer.h"
#include "Material.h"
#include <vector>

class Mesh
{
public:
	Mesh()
	{
		InitMesh();
	}

	Mesh(Transform InTransform)
		:MeshTransform(InTransform)
	{
		InitMesh();
	}

	void GetElementInfo(std::vector<MeshVertex>& OutVertices, std::vector<int>& OutIndices);

	Material* GetMaterial();

	void SetMaterial(Material* InMaterial);

protected:
	virtual void InitMesh() {};

public:

	Transform MeshTransform;

public:

	std::vector<MeshVertex> Vertices;

	std::vector<int> Indices;

private:

	Material* Mat;
};



