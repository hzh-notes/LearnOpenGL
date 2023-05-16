#pragma once

#ifndef __TMatrix
#define __TMatrix

#include <vector>
#include "Vector.h"

template <typename T>
struct TMatrix
{
public:

	TMatrix();

	TMatrix(std::vector<T> Values);
public:

	TMatrix<T> operator*(const TMatrix<T>& Other) const;

	T Determinant() const;

	TMatrix<T> Inverse() const;

	TVector4<T> Row(int RowNum) const;

	TVector4<T> Column(int ColumnNum) const;

	TVector<T> TransformPosition(const TVector<T>& V) const;

	TVector4<T> TransformVector4(const TVector4<T>& V) const;

	TVector4<T> InverseTransformVector4(const TVector4<T>& V) const;
	
	static TMatrix<T> Indentity();

	static TMatrix<T> Translate(TVector<T> Tranlation);

	static TMatrix<T> RotateX(T Angle);

	static TMatrix<T> RotateY(T Angle);

	static TMatrix<T> RotateZ(T Angle);

	static TMatrix<T> Rotate(TVector<T> Rotation);

	static TMatrix<T> Scale(TVector<T> Scale3D);

	static TMatrix<T> LookAt(TVector<T> Eye, TVector<T> Target, TVector<T> Up);

	static TMatrix<T> OrthoMatrix(T Width, T Height, T MinZ, T MaxZ);

	static TMatrix<T> ReversedZOrthoMatrix(T Width, T Height, T MinZ, T MaxZ);

	static TMatrix<T> Perspective(T HalfFOV, T Width, T Height, T MinZ, T MaxZ);

	static TMatrix<T> ReversedZPerspective(T HalfFOV, T Width, T Height, T MinZ, T MaxZ);

public:
	T M[16];
};

typedef TMatrix<float> Matrixf;
typedef TMatrix<double> Matrixd;

#endif


