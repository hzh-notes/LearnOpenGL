#include "Transform.h"

Matrix Transform::GetMatrixWithScale() const
{
	Matrix TranslateMat = Matrix::Translate(Position);
	Matrix RotateMat = Rotation.ToMatrix();
	Matrix ScaleMat = Matrix::Scale(Scale3D);

	return ScaleMat * RotateMat * TranslateMat;
}

Matrix Transform::GetMatrixWithoutScale() const
{
	Matrix TranslateMat = Matrix::Translate(Position);
	Matrix RotateMat = Rotation.ToMatrix();

	return  RotateMat * TranslateMat;
}

Matrix Transform::GetRotationMatrix() const
{
	return Rotation.ToMatrix();
}

Vector3f Transform::TransformPosition(Vector3f V) const
{
	return GetMatrixWithScale().TransformPosition(V);
}

Vector4f Transform::TransformVector4f(Vector4f V) const
{
	return GetMatrixWithScale().TransformVector4f(V);
}
