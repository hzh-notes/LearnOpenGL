#pragma once
#include "Transform.h"
class Mesh
{
public:
	Mesh() {}

	Mesh(Transform InTransform)
		:transform(InTransform)
	{}

public:

	Transform transform;
};

