#pragma once

#ifndef __VECTOR3_H
#define __VECTOR3_H

template <typename T> struct TVector2;
template <typename T> struct TVector4;

template <typename T>
struct TVector
{
public:
	TVector() :X(0), Y(0), Z(0) {};

	TVector(T Value) :X(Value), Y(Value), Z(Value) {};

	TVector(T InX, T InY, T InZ) :X(InX), Y(InY), Z(InZ) {};

	TVector(const TVector2<T>& InVector2);

	TVector(const TVector4<T>& InVector4);

public:
	TVector<T> operator+(const TVector<T>& Orther) const;

	TVector<T> operator-(const TVector<T>& Orther) const;

	TVector<T> operator*(const TVector<T>& Orther) const;

	TVector<T> operator*(const T Value) const;

	TVector<T> operator/(const TVector<T>& Orther) const;

	TVector<T> operator/(T Value) const;

	T operator|(const TVector<T>& Orther) const;

	TVector<T> operator+=(const TVector<T>& Orther);

	TVector<T> operator^(const TVector<T>& Orther) const;

	TVector<T> Normalize();

public:

	T X;
	T Y;
	T Z;
};

typedef TVector<float> Vector3f;
typedef TVector<double> Vector3d;
typedef TVector<int> Vector3i;

#endif



