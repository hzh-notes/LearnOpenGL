#pragma once

#ifndef __MATRIX
#define __MATRIX

#include <vector>
#include "Vector.h"

struct Matrix
{
public:

	Matrix();

	Matrix(std::vector<float> Values);
public:

	Matrix operator*(const Matrix& Other) const;

	float Determinant() const;

	Matrix Inverse() const;

	Vector4f Row(int RowNum) const;

	Vector4f Column(int ColumnNum) const;

	Vector3f TransformPosition(const Vector3f& V) const;

	Vector4f TransformVector4f(const Vector4f& V) const;

	Vector4f InverseTransformVector4f(const Vector4f& V) const;
	
	static Matrix Indentity();

	static Matrix Translate(Vector3f Tranlation);

	static Matrix RotateX(float Angle);

	static Matrix RotateY(float Angle);

	static Matrix RotateZ(float Angle);

	static Matrix Rotate(Vector3f Rotation);

	static Matrix Scale(Vector3f Scale3D);

	static Matrix LookAt(Vector3f Eye, Vector3f Target, Vector3f Up);

	static Matrix OrthoMatrix(float Width, float Height, float MinZ, float MaxZ);

	static Matrix ReversedZOrthoMatrix(float Width, float Height, float MinZ, float MaxZ);

	static Matrix Perspective(float HalfFOV, float Width, float Height, float MinZ, float MaxZ);

	static Matrix ReversedZPerspective(float HalfFOV, float Width, float Height, float MinZ, float MaxZ);

public:
	float M[16];
};

#endif


