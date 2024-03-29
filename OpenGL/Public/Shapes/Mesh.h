#pragma once
#include "Math/Transform.h"
#include "RenderCore/VertexBuffer.h"
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

	~Mesh();

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



