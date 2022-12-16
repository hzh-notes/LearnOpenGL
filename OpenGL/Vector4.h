#pragma once

#ifndef __VECTOR4_H
#define __VECTOR4_H

template <typename T> struct TVector2;
template <typename T> struct TVector;

template <typename T>
struct TVector4
{
public:
	TVector4() :X(0.0), Y(0.0), Z(0.0), W(0.0) {};

	TVector4(T InX, T InY, T InZ, T InW) :X(InX), Y(InY), Z(InZ), W(InW) {};

	TVector4(const TVector2<T>& InVector2);

	TVector4(const TVector<T>& InVector);

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


typedef TVector4<float> Vector4f;
typedef TVector4<double> Vector4d;
typedef TVector4<int> Vector4i;

#endif



