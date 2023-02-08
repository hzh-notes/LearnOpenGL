#pragma once
#include "Transform.h"
#include "VertexBuffer.h"
#include <vector>

class Mesh
{
public:
	Mesh()
	{
		InitMesh();
	}

	Mesh(Transform InTransform)
		:transform(InTransform)
	{
		InitMesh();
	}

protected:
	virtual void InitMesh() {};

	void GetElementInfo(std::vector<VertexDescription>& OutVertices, std::vector<int>& OutIndices);

public:

	Transform transform;

public:

	std::vector<Vector3f> positions;

	std::vector<Vector2f> uvs;

	std::vector<int> indices;

};



