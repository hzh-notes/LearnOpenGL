#include "Vector3.h"
#include <math.h>

template class TVector<float>;
template class TVector<double>;
template class TVector<int>;

template <typename T>
TVector<T> TVector<T>::operator+(const TVector<T>& Orther) const
{
	return TVector(X + Orther.X, Y + Orther.Y, Z + Orther.Z);
}

template <typename T>
TVector<T> TVector<T>::operator-(const TVector<T>& Orther) const
{
	return TVector(X - Orther.X, Y - Orther.Y, Z - Orther.Z);
}

template <typename T>
TVector<T> TVector<T>::operator*(const TVector<T>& Orther) const
{
	return TVector<T>(X * Orther.X, Y * Orther.Y, Z * Orther.Z);
}

template <typename T>
TVector<T> TVector<T>::operator/(const TVector<T>& Orther) const
{
	return TVector(X / Orther.X, Y / Orther.Y, Z / Orther.Z);
}

template <typename T>
TVector<T> TVector<T>::operator/(T Value) const
{
	return TVector(X / Value, Y / Value, Z / Value);
}

template <typename T>
T TVector<T>::operator|(const TVector<T>& Orther) const
{
	return X * Orther.X + Y * Orther.Y + Z * Orther.Z;
}

template <typename T>
TVector<T> TVector<T>::operator^(TVector<T>& Orther) const
{
	return TVector(
		Y * Orther.Z - Z * Orther.Y,
		Z * Orther.X - X * Orther.Z,
		X * Orther.Y - Y * Orther.X);
}

template <typename T>
TVector<T> TVector<T>::Normalize()
{
	T LenghtSqrt = X * X + Y * Y + Z * Z;

	return (*this) / sqrt(LenghtSqrt);
}