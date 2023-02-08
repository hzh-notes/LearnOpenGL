#pragma once
#include "Vector.h"
#include "Matrix.h"

class Transform
{
public:
	Transform()
		:Position(Vector3f())
		, Rotation(Vector3f())
		, Scale3D(Vector3f(1.f))
	{}

	Transform(Vector3f InPosition)
		:Position(InPosition)
		, Rotation(Vector3f())
		, Scale3D(Vector3f(1.f))
	{}

	Transform(Vector3f InPosition, Vector3f InRotation)
		:Position(InPosition)
		, Rotation(InRotation)
		, Scale3D(Vector3f(1.f))
	{}

	Transform(Vector3f InPosition, Vector3f InRotation, Vector3f InScale3D)
		:Position(InPosition)
		, Rotation(InRotation)
		, Scale3D(InScale3D)
	{}

public:

	Matrix GetMatrixWithScale();

	Matrix GetMatrixWithoutScale();

public:
	Vector3f Position;
	Vector3f Rotation;
	Vector3f Scale3D;
};

