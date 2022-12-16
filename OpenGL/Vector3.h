#pragma once

#ifndef __VECTOR3_H
#define __VECTOR3_H

template <typename T> class TVector2;
template <typename T> class TVector4;

template <typename T>
class TVector
{
public:
	TVector() :X(0), Y(0), Z(0) {};

	TVector(T InX, T InY, T InZ) :X(InX), Y(InY), Z(InZ) {};

	TVector(const TVector2<T>& InVector2);

	TVector(const TVector4<T>& InVector4);

public:
	TVector<T> operator+(const TVector<T>& Orther) const;

	TVector<T> operator-(const TVector<T>& Orther) const;

	TVector<T> operator*(const TVector<T>& Orther) const;

	TVector<T> operator/(const TVector<T>& Orther) const;

	T operator|(const TVector<T>& Orther) const;

	TVector<T> operator^(TVector<T>& Orther) const;

public:

	T X;
	T Y;
	T Z;
};

typedef TVector<float> Vector3f;
typedef TVector<double> Vector3d;
typedef TVector<int> Vector3i;

#endif



