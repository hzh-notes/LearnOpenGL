#include "Vector4.h"

template class TVector4<float>;
template class TVector4<double>;
template class TVector4<int>;

template <typename T>
TVector4<T> TVector4<T>::operator+(const TVector4<T>& Orther) const
{
	return TVector4(X + Orther.X, Y + Orther.Y, Z + Orther.Z, W + Orther.W);
}

template <typename T>
TVector4<T> TVector4<T>::operator-(const TVector4<T>& Orther) const
{
	return TVector4(X - Orther.X, Y - Orther.Y, Z - Orther.Z, W - Orther.W);
}

template <typename T>
TVector4<T> TVector4<T>::operator*(const TVector4<T>& Orther) const
{
	return TVector4<T>(X * Orther.X, Y * Orther.Y, Z * Orther.Z, W * Orther.W);
}

template <typename T>
TVector4<T> TVector4<T>::operator/(const TVector4<T>& Orther) const
{
	return TVector4(X / Orther.X, Y / Orther.Y, Z / Orther.Z, W / Orther.W);
}

template <typename T>
T TVector4<T>::operator|(const TVector4<T>& Orther) const
{
	return X * Orther.X + Y * Orther.Y + Z * Orther.Z + W * Orther.W;
}

template <typename T>
TVector4<T> TVector4<T>::operator^(TVector4<T>& Orther) const
{
	return TVector4(
		Y * Orther.Z - Z * Orther.Y,
		Z * Orther.X - X * Orther.Z,
		X * Orther.Y - Y * Orther.X,
		T(0.0));
}
