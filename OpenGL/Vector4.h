#pragma once

#ifndef __VECTOR4_H
#define __VECTOR4_H

#include "Vector3.h"

template <typename T>
class TVector4
{
public:
	TVector4() :X(0), Y(0), Z(0), W(0) {};
	TVector4(T InX, T InY, T InZ, T InW) :X(InX), Y(InY), Z(InZ), W(InW) {};

public:

	TVector4<T> operator+(const TVector4<T>& Orther) const;

	TVector4<T> operator-(const TVector4<T>& Orther) const;

	TVector4<T> operator*(const TVector4<T>& Orther) const;

	TVector4<T> operator/(const TVector4<T>& Orther) const;

	T operator|(const TVector4<T>& Orther) const;

	TVector4<T> operator^(TVector4<T>& Orther) const;

public:

	T X;
	T Y;
	T Z;
	T W;
};

//template <typename T>
//TVector<T>::TVector(const TVector4<T>& InVector4)
//	:X(InVector4.X)
//	,Y(InVector4.Y)
//	,Z(InVector4.Z)
//{
//	
//}

typedef TVector4<float> Vector4f;
typedef TVector4<double> Vector4d;
typedef TVector4<int> Vector4i;

#endif



