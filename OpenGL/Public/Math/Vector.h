#pragma once

#ifndef __VECTOR_H__
#define __VECTOR_H__ 

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

template <typename T>
TVector2<T>::TVector2(const TVector<T>& InVector)
	: X(InVector.X)
	, Y(InVector.Y)
{

}

template <typename T>
TVector2<T>::TVector2(const TVector4<T>& InVector4)
	: X(InVector4.X)
	, Y(InVector4.Y)
{

}

template <typename T>
TVector<T>::TVector(const TVector2<T>& InVector2)
	: X(InVector2.X)
	, Y(InVector2.Y)
	, Z(0.0)
{

}

template <typename T>
TVector<T>::TVector(const TVector4<T>& InVector4)
	: X(InVector4.X)
	, Y(InVector4.Y)
	, Z(InVector4.Z)
{

}

template <typename T>
TVector4<T>::TVector4(const TVector2<T>& InVector2)
	: X(InVector2.X)
	, Y(InVector2.Y)
	, Z(0.0)
	, W(0.0)
{

}
template <typename T>
TVector4<T>::TVector4(const TVector<T>& InVector, float InW)
	: X(InVector.X)
	, Y(InVector.Y)
	, Z(InVector.Z)
	, W(InW)
{

}

template <typename T>
TVector4<T>::TVector4(const TVector<T>& InVector3)
	: X(InVector3.X)
	, Y(InVector3.Y)
	, Z(InVector3.Z)
	, W(0.0)
{

}
#endif //__VECTOR_H__

