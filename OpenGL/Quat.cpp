#include "Quat.h"

template class TQuat<float>;

template<typename T>
T ClampAxis(T Angle)
{
	//[-360.0, 360.0]
	Angle = fmod(Angle, (T)360.0);

	if (Angle < (T)0.0)
	{
		Angle += (T)360.0;
	}

	return Angle;
}

template<typename T>
T NormalizeAxis(T Angle)
{
	// returns Angle in the range [0,360)
	Angle = ClampAxis(Angle);

	if (Angle > (T)180.0)
	{
		// shift to (-180,180]
		Angle -= (T)360.0;
	}

	return Angle;
}

template<typename T>
TQuat<T>::TQuat(const TVector<T>& Axis, T& AngleRad)
{
	const T half_a = (T)0.5 * AngleRad;
	T s, c;
	s = sin(half_a);
	c = cos(half_a);

	X = s * Axis.X;
	Y = s * Axis.Y;
	Z = s * Axis.Z;
	W = c;
}

template<typename T>
TQuat<T> TQuat<T>::operator*(const TQuat<T>& InQuat)
{
	TQuat<T> Result;
	return Result;
}

template<typename T>
TVector<T> TQuat<T>::ToRotation() const
{
	const T SingularityTest = Z * X - W * Y;
	const T YawY = (T)2.0 * (W * Z + X * Y);
	const T YawX = ((T)1.0 - (T)2.0 * (Y * Y + Z * Z));

	const T SINGULARITY_THRESHOLD = (T)0.4999995;
	const T RAD_TO_DEG = (T)(180.0 / 3.1415926);
	T Roll, Pitch, Yaw;

	if (SingularityTest < -SINGULARITY_THRESHOLD)
	{
		Pitch = (T)-90.0;
		Yaw = (atan2(YawY, YawX) * RAD_TO_DEG);
		Roll = NormalizeAxis(-Yaw - ((T)2.0 * atan2(X, W) * RAD_TO_DEG));
	}
	else if (SingularityTest > SINGULARITY_THRESHOLD)
	{
		Pitch = (T)90.0;
		Yaw = (atan2(YawY, YawX) * RAD_TO_DEG);
		Roll = NormalizeAxis(Yaw - ((T)2.0 * atan2(X, W) * RAD_TO_DEG));
	}
	else
	{
		Pitch = (asin((T)2.0 * SingularityTest) * RAD_TO_DEG); // Note: not FastAsin like float implementation
		Yaw = (atan2(YawY, YawX) * RAD_TO_DEG);
		Roll = (atan2((T)-2.0 * (W * X + Y * Z), ((T)1.0 - (T)2.0 * (X * X + Y * Y))) * RAD_TO_DEG);
	}

	TVector<T> RotatorFromQuat = TVector<T>(Roll, Pitch, Yaw);

	return RotatorFromQuat;
}

