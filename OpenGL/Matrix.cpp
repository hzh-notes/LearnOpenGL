#include "Matrix.h"
#include <math.h>

#define PI 3.1415926f

Matrix::Matrix()
{
	for (int i = 0; i < 16; ++i)
	{
		M[i] = 0.f;
	}
}

Matrix::Matrix(std::vector<float> Values)
{
	for (int i = 0; i < 16; ++i)
	{
		M[i] = i < Values.size() ? Values[i] : 0.f;
	}
}

Matrix Matrix::operator*(const Matrix& Other) const
{
	Matrix NewMatrix;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			NewMatrix.M[i * 4 + j] = this->Row(i) | Other.Column(j);
		}
	}

	return NewMatrix;
}

Vector4f Matrix::Row(int RowNum) const
{

	return Vector4f(M[RowNum * 4], M[RowNum * 4 + 1], M[RowNum * 4 + 2], M[RowNum * 4 + 3]);
}

Vector4f Matrix::Column(int ColumnNum) const
{
	return Vector4f(M[ColumnNum], M[4 + ColumnNum], M[8 + ColumnNum], M[12 + ColumnNum]);
}

Vector3f Matrix::TransformPosition(const Vector3f& V) const
{
	Vector4f Result = TransformVector4f(Vector4f(V, 1.f));

	return Vector3f(Result) / Result.W;
}

Vector4f Matrix::TransformVector4f(const Vector4f& V) const
{
	float X = V | Column(0);
	float Y = V | Column(1);
	float Z = V | Column(2);
	float W = V | Column(3);

	return Vector4f(X, Y, Z, W);
}

Matrix Matrix::Indentity()
{
	return Matrix({
		1.f, 0.f, 0.f,0.f,
		0.f, 1.f, 0.f,0.f,
		0.f, 0.f, 1.f,0.f,
		0.f, 0.f, 0.f,1.f });
}

Matrix Matrix::Translate(Vector3f Tranlation)
{
	return Matrix(
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			Tranlation.X, Tranlation.Y, Tranlation.Z, 1.0f
		});
}

Matrix Matrix::RotateX(float Angle)
{
	float SinX = sin(Angle * PI / 180.f);
	float CosX = cos(Angle * PI / 180.f);
	return Matrix(
		{
			1.f, 0.f, 0.f,0.f,
			0.f, CosX, -SinX,0.f,
			0.f, SinX, CosX,0.f,
			0.f, 0.f, 0.f,1.f
		});
}

Matrix Matrix::RotateY(float Angle)
{
	float SinY = sin(Angle * PI / 180.f);
	float CosY = cos(Angle * PI / 180.f);
	return Matrix(
		{
			CosY, 0.f, SinY,0.f,
			0.f, 1.f, 0.f,0.f,
			-SinY, 0.f, CosY,0.f,
			0.f, 0.f, 0.f,1.f
		});
}

Matrix Matrix::RotateZ(float Angle)
{
	float SinZ = sin(Angle * PI / 180.f);
	float CosZ = cos(Angle * PI / 180.f);
	return Matrix(
		{
			CosZ, SinZ, 0.f,0.f,
			-SinZ, CosZ, 0.f,0.f,
			0.f, 0.f, 1.f,0.f,
			0.f, 0.f, 0.f,1.f
		});
}

Matrix Matrix::Rotate(Vector3f Rotation)
{
	return RotateZ(Rotation.Z) * RotateY(Rotation.Y) * RotateX(Rotation.X);
}

Matrix Matrix::Scale(Vector3f Scale3D)
{
	return Matrix(
		{
			Scale3D.X, 0.0f, 0.0f, 0.0f,
			0.0f, Scale3D.Y, 0.0f, 0.0f,
			0.0f, 0.0f, Scale3D.Z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		});
}

Matrix Matrix::LookAt(Vector3f Eye, Vector3f Target, Vector3f Up)
{
	Vector3f AxisZ = Target.Normalize();
	Vector3f AxisX = (Up ^ AxisZ).Normalize();
	Vector3f AxisY = (AxisZ ^ AxisX).Normalize();

	float EyeX = - (AxisX | Eye);
	float EyeY = - (AxisY | Eye);
	float EyeZ = - (AxisZ | Eye);

	return Matrix(
		{
			AxisX.X, AxisY.X, AxisZ.X, 0.f,
			AxisX.Y, AxisY.Y, AxisZ.Y, 0.f,
			AxisX.Z, AxisY.Z, AxisZ.Z, 0.f,
			EyeX, EyeY, EyeZ, 1.f
		});
}

Matrix Matrix::Perspective(float HalfFOV, float Width, float Height, float MinZ, float MaxZ)
{
	HalfFOV = HalfFOV * PI / 180.0f;
	return Matrix(
		{
			1.0f / (float)tan(HalfFOV), 0.0f, 0.0f, 0.0f,
			0.0f, Width / (float)tan(HalfFOV) / Height, 0.0f, 0.0f,
			0.0f, 0.0f, ((MinZ == MaxZ) ? 1.0f : MaxZ / (MaxZ - MinZ)), 1.0f,
			0.0f, 0.0f, ((MinZ == MaxZ) ? -MinZ : -MaxZ * MinZ / (MaxZ - MinZ)), 0.0f
		});
}

Matrix Matrix::ReversedZPerspective(float HalfFOV, float Width, float Height, float MinZ, float MaxZ)
{
	HalfFOV = HalfFOV * PI / 180.0f;
	return Matrix(
		{
			1.0f / (float)tan(HalfFOV), 0.0f, 0.0f, 0.0f,
			0.0f, Width / (float)tan(HalfFOV) / Height, 0.0f, 0.0f,
			0.0f, 0.0f, ((MinZ == MaxZ) ? 0.0f : MinZ / (MinZ - MaxZ)), 1.0f,
			0.0f, 0.0f, ((MinZ == MaxZ) ? MinZ : -MaxZ * MinZ / (MinZ - MaxZ)), 0.0f
		});
}
