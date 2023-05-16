#pragma once
#include "Vector.h"
#include "Quat.h"

class Transform
{
public:
	Transform()
		:Position(Vector3f())
		, Rotation(Quat4f())
		, Scale3D(Vector3f(1.f))
	{}

	Transform(Vector3f InPosition)
		:Position(InPosition)
		, Rotation(Quat4f())
		, Scale3D(Vector3f(1.f))
	{}

	Transform(Vector3f InPosition, Vector3f InRotation)
		:Position(InPosition)
		, Rotation(Quat4f::FromEuler(InRotation))
		, Scale3D(Vector3f(1.f))
	{}

	Transform(Vector3f InPosition, Vector3f InRotation, Vector3f InScale3D)
		:Position(InPosition)
		, Rotation(Quat4f::FromEuler(InRotation))
		, Scale3D(InScale3D)
	{}

public:

	Matrixf GetMatrixWithScale() const;

	Matrixf GetMatrixWithoutScale() const;

	Matrixf GetRotationMatrix() const;

	Vector3f TransformPosition(Vector3f V) const;

	Vector4f TransformVector4f(Vector4f V) const;

	Vector3f GetForwardVector() const;

	Vector3f GetUpVector() const;

	Vector3f GetRightVector() const;
public:
	Vector3f Position;
	Quat4f Rotation;
	Vector3f Scale3D;
};

