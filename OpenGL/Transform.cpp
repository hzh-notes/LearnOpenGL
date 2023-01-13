#include "Transform.h"

Matrix Transform::GetMatrixWithScale()
{
	Matrix TranslateMat = Matrix::Translate(Position);
	Matrix RotateMat = Matrix::Rotate(Rotation);
	Matrix ScaleMat = Matrix::Scale(Scale3D);

	return ScaleMat * RotateMat * TranslateMat;
}

Matrix Transform::GetMatrixWithoutScale()
{
	Matrix TranslateMat = Matrix::Translate(Position);
	Matrix RotateMat = Matrix::Rotate(Rotation);
	Matrix ScaleMat = Matrix::Scale(Vector3f());

	return ScaleMat * RotateMat * TranslateMat;
}
