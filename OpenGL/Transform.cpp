#include "Transform.h"

Matrix Transform::GetMatrixWithScale() const
{
	Matrix TranslateMat = Matrix::Translate(Position);
	Matrix RotateMat = Matrix::Rotate(Rotation);
	Matrix ScaleMat = Matrix::Scale(Scale3D);

	return ScaleMat * RotateMat * TranslateMat;
}

Matrix Transform::GetMatrixWithoutScale() const
{
	Matrix TranslateMat = Matrix::Translate(Position);
	Matrix RotateMat = Matrix::Rotate(Rotation);

	return RotateMat * TranslateMat;
}

Matrix Transform::GetRotationMatrix() const
{
	return Matrix::Rotate(Rotation);
}
