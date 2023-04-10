#pragma once

#ifndef __VERTEXBUFFER
#define __VERTEXBUFFER

#include "Math/Vector.h"

struct MeshVertex
{
public:
	MeshVertex()
		: Position(Vector3f())
		, Normal(Vector3f())
		, Color(Vector4f(1))
		, UV(Vector2f())
	{}

	MeshVertex(Vector3f InPosition,Vector3f InNormal, Vector4f InColor, Vector2f InUV)
		: Position(InPosition)
		, Normal(InNormal)
		, Color(InColor)
		, UV(InUV)
	{}

	Vector3f Position;

	Vector3f Normal;

	Vector4f Color;

	Vector2f UV;
};

class VertexBuffer
{
public:
	VertexBuffer(const MeshVertex* Vertices, int Size);

	void SetVertexElement(unsigned int Index, unsigned int Size, unsigned int Type, unsigned int Stride, unsigned int Offset, bool Normalized);

public:

	unsigned int BufferId = -1;
};

#endif // !__VERTEXBUFFER




