#pragma once

#ifndef __QUAT_H
#define __QUAT_H
#include "Vector.h"
#include "Matrix.h"

template<typename T>
struct TQuat
{
public:
	TQuat<T>()
		: X((T)0.0)
		, Y((T)0.0)
		, Z((T)0.0)
		, W((T)1.0)
	{}

	TQuat<T>(T AxisX, T AxisY, T AxisZ, T AngleRad);

	TQuat<T>(const TVector<T>& Axis, T AngleRad);

	TQuat<T> operator*(const TQuat<T>& InQuat);

	TVector<T> RotateVector(TVector<T> V) const;

	TVector<T> UnrotateVector(TVector<T> V) const;

	TVector<T> ToRotation() const;

	Matrix ToMatrix() const;

	static TQuat<T> FromEuler(TVector<T>& Euler);

public:

	T X;
	T Y;
	T Z;
	T W;
};

typedef TQuat<float> Quat4f;
typedef TQuat<double> Quat4d;



#endif


