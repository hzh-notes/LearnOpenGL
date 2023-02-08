#pragma once

#ifndef __VERTEXBUFFER
#define __VERTEXBUFFER

#include "Vector.h"

struct VertexDescription
{
public:
	VertexDescription()
		:Position(Vector3f())
		,Color(Vector4f(1))
		,UV(Vector2f())
	{}

	VertexDescription(Vector3f InPosition, Vector4f InColor, Vector2f InUV)
		: Position(InPosition)
		, Color(InColor)
		, UV(InUV)
	{}

	Vector3f Position;

	Vector4f Color;

	Vector2f UV;
};

class VertexBuffer
{
public:
	VertexBuffer(const VertexDescription* Vertices, int Size);

public:

	unsigned int BufferId = -1;
};

#endif // !__VERTEXBUFFER




