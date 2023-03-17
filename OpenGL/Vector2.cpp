#include "Vector2.h"

template class TVector2<float>;
template class TVector2<double>;
template class TVector2<int>;

template <typename T>
TVector2<T> TVector2<T>::operator+(const TVector2<T>& Orther) const
{
	return TVector2(X + Orther.X, Y + Orther.Y);
}

template <typename T>
TVector2<T> TVector2<T>::operator-(const TVector2<T>& Orther) const
{
	return TVector2(X - Orther.X, Y - Orther.Y);
}

template <typename T>
TVector2<T> TVector2<T>::operator*(const TVector2<T>& Orther) const
{
	return TVector2<T>(X * Orther.X, Y * Orther.Y);
}

template <typename T>
TVector2<T> TVector2<T>::operator/(const TVector2<T>& Orther) const
{
	return TVector2(X / Orther.X, Y / Orther.Y);
}

template <typename T>
T TVector2<T>::operator|(const TVector2<T>& Orther) const
{
	return X * Orther.X + Y * Orther.Y;
}

template <typename T>
T TVector2<T>::operator^(const TVector2<T>& Orther) const
{
	return X * Orther.Y - Y * Orther.X;
}
