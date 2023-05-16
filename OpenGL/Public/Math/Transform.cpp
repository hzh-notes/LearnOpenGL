#include "Transform.h"

Matrixf Transform::GetMatrixWithScale() const
{
	Matrixf TranslateMat = Matrixf::Translate(Position);
	Matrixf RotateMat = Rotation.ToMatrix();
	Matrixf ScaleMat = Matrixf::Scale(Scale3D);

	return ScaleMat * RotateMat * TranslateMat;
}

Matrixf Transform::GetMatrixWithoutScale() const
{
	Matrixf TranslateMat = Matrixf::Translate(Position);
	Matrixf RotateMat = Rotation.ToMatrix();

	return  RotateMat * TranslateMat;
}

Matrixf Transform::GetRotationMatrix() const
{
	return Rotation.ToMatrix();
}

Vector3f Transform::TransformPosition(Vector3f V) const
{
	return GetMatrixWithScale().TransformPosition(V);
}

Vector4f Transform::TransformVector4f(Vector4f V) const
{
	return GetMatrixWithScale().TransformVector4(V);
}

Vector3f Transform::GetForwardVector() const
{
	return GetMatrixWithoutScale().TransformVector4(Vector3f(1, 0, 0));
}

Vector3f Transform::GetUpVector() const
{
	return GetMatrixWithoutScale().TransformVector4(Vector3f(0, 0, 1));
}

Vector3f Transform::GetRightVector() const
{
	return GetMatrixWithoutScale().TransformVector4(Vector3f(0, 1, 0));
}
