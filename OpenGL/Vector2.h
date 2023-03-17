#pragma once

#ifndef __VECTOR2_H
#define __VECTOR2_H

template <typename T> struct TVector;
template <typename T> struct TVector4;

template <typename T>
struct TVector2
{
public:
	TVector2() :X(0.0), Y(0.0) {};

	TVector2(T InX, T InY) :X(InX), Y(InY) {};

	TVector2(const TVector<T>& InVector);

	TVector2(const TVector4<T>& InVector4);

public:

	TVector2<T> operator+(const TVector2<T>& Orther) const;

	TVector2<T> operator-(const TVector2<T>& Orther) const;

	TVector2<T> operator*(const TVector2<T>& Orther) const;

	TVector2<T> operator/(const TVector2<T>& Orther) const;

	T operator|(const TVector2<T>& Orther) const;

	T operator^(const TVector2<T>& Orther) const;

public:

	T X;
	T Y;
};


typedef TVector2<float> Vector2f;
typedef TVector2<double> Vector2d;
typedef TVector2<int> Vector2i;


#endif // !__VECTOR2_H




