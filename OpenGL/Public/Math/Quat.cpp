#include "Quat.h"
#include "Macro.h"

template class TQuat<float>;
template class TQuat<double>;

template<typename T>
TQuat<T>::TQuat<T>(T AxisX, T AxisY, T AxisZ, T AngleRad)
	: X(AxisX)
	, Y(AxisY)
	, Z(AxisZ)
	, W(AngleRad)
{
	const T half_a = (T)0.5 * AngleRad;
	T s, c;
	s = sin(half_a);
	c = cos(half_a);

	X = s * AxisX;
	Y = s * AxisY;
	Z = s * AxisZ;
	W = c;
}

template<typename T>
TQuat<T>::TQuat(const TVector<T>& Axis, T AngleRad)
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

	Result.W = W * InQuat.W - X * InQuat.X - Y * InQuat.Y - Z * InQuat.Z;
	Result.X = X * InQuat.W + Y * InQuat.Z - Z * InQuat.Y + W * InQuat.X;
	Result.Y = Y * InQuat.W + W * InQuat.Y + Z * InQuat.X - X * InQuat.Z;
	Result.Z = Z * InQuat.W + W * InQuat.Z + X * InQuat.Y - Y * InQuat.X;

	return Result;
}

template<typename T>
TVector<T> TQuat<T>::RotateVector(TVector<T> V) const
{
	const TVector<T> Q(X, Y, Z);
	const TVector<T> TT = TVector<T>(2.0) * (Q ^ V);
	const TVector<T> Result = V + (TVector<T>(W) * TT) + Q ^ TT;
	return Result;
}

template<typename T>
TVector<T> TQuat<T>::UnrotateVector(TVector<T> V) const
{
	const TVector<T> Q(-X, -Y, -Z);
	const TVector<T> TT = TVector<T>(2.0) * (Q ^ V);
	const TVector<T> Result = V + (TVector<T>(W) * TT) + Q ^ TT;
	return Result;
}

template<typename T>
TVector<T> TQuat<T>::ToRotation() const
{
	const T SingularityTest = Z * X - W * Y;
	const T YawY = (T)2.0 * (W * Z + X * Y);
	const T YawX = ((T)1.0 - (T)2.0 * (Y * Y + Z * Z));

	const T SINGULARITY_THRESHOLD = (T)0.4999995;
	const T RAD_TO_DEG = RadToDeg;
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

template<typename T>
TMatrix<T> TQuat<T>::ToMatrix() const
{
	TMatrix<T> R;
	const T x2 = X + X;    const T y2 = Y + Y;    const T z2 = Z + Z;
	const T xx = X * x2;   const T xy = X * y2;   const T xz = X * z2;
	const T yy = Y * y2;   const T yz = Y * z2;   const T zz = Z * z2;
	const T wx = W * x2;   const T wy = W * y2;   const T wz = W * z2;

	R.M[0]  = T(1.0) - (yy + zz);	R.M[1]  = xy - wz;				R.M[2]  = xz + wy;				R.M[3]  = T(0.0);
	R.M[4]  = xy + wz;				R.M[5]  = T(1.0) - (xx + zz);	R.M[6]  = yz - wx;				R.M[7]  = T(0.0);
	R.M[8]  = xz - wy;				R.M[9]  = yz + wx;				R.M[10] = T(1.0) - (xx + yy);	R.M[11] = T(0.0);
	R.M[12] = T(0.0);				R.M[13] = T(0.0);				R.M[14] = T(0.0);				R.M[15] = T(1.0);

	return R;
}

template<typename T>
TQuat<T> TQuat<T>::FromEuler(TVector<T>& Euler)
{
	TQuat<T> QuatFromRoll = TQuat<T>(1.0, 0.0, 0.0, DegToRad * Euler.X);
	TQuat<T> QuatFromPitch = TQuat<T>(TVector<T>(0.0, -1.0, 0.0), DegToRad * Euler.Y);
	TQuat<T> QuatFromYaw = TQuat<T>(TVector<T>(0.0, 0.0, 1.0), DegToRad * Euler.Z);

	return QuatFromRoll * QuatFromPitch * QuatFromYaw;
}
