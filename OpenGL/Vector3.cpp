#include "Vector3.h"

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

