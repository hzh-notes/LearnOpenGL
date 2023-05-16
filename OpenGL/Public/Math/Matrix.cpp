#include "Matrix.h"
#include "Macro.h"

template struct TMatrix<float>;
template struct TMatrix<double>;

template<typename T>
TMatrix<T>::TMatrix<T>()
{
	for (int i = 0; i < 16; ++i)
	{
		M[i] = 0.f;
	}
}

template<typename T>
TMatrix<T>::TMatrix<T>(std::vector<T> Values)
{
	for (int i = 0; i < 16; ++i)
	{
		M[i] = i < Values.size() ? Values[i] : 0.f;
	}
}

template<typename T>
TMatrix<T> TMatrix<T>::operator*(const TMatrix<T>& Other) const
{
	TMatrix<T> NewMatrix;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			NewMatrix.M[i * 4 + j] = this->Row(i) | Other.Column(j);
		}
	}

	return NewMatrix;
}

template<typename T>
T TMatrix<T>::Determinant() const
{
	return M[0] * (
		M[5] * (M[10] * M[15] - M[11] * M[14]) -
		M[6] * (M[9] * M[15] - M[10] * M[13]) +
		M[7] * (M[9] * M[14] - M[10] * M[13])
		) -
		M[1] * (
			M[4] * (M[10] * M[15] - M[11] * M[14]) - 
			M[6] * (M[8] * M[15] - M[11] * M[12]) + 
			M[7] * (M[8] * M[14] - M[10] * M[12])
			) +
		M[2] * (
			M[4] * (M[9] * M[15] - M[10] * M[13]) - 
			M[5] * (M[8] * M[15] - M[11] * M[12]) + 
			M[7] * (M[8] * M[13] - M[9] * M[12])
			) -
		M[3] * (
			M[4] * (M[9] * M[14] - M[10] * M[13]) - 
			M[5] * (M[8] * M[14] - M[10] * M[12]) + 
			M[6] * (M[8] * M[13] - M[9] * M[12])
			);
}

template<typename T>
TMatrix<T> TMatrix<T>::Inverse() const
{
	TMatrix<T> Result;
	const T Det = Determinant();

	if (Det == 0.0f)
	{
		Result = Indentity();
	}
	else
	{
		T Det[4];
		T Tmp[12];

		Tmp[0] = M[10] * M[15] - M[14] * M[11];
		Tmp[4] = M[9] * M[15] - M[13] * M[11];
		Tmp[8] = M[9] * M[14] - M[13] * M[10];

		Tmp[1] = M[10] * M[15] - M[14] * M[11];
		Tmp[5] = M[8] * M[15] - M[12] * M[11];
		Tmp[9] = M[8] * M[14] - M[12] * M[10];

		Tmp[2] = M[9] * M[15] - M[13] * M[11];
		Tmp[6] = M[8] * M[15] - M[12] * M[11];
		Tmp[10] = M[8] * M[13] - M[12] * M[9];

		Tmp[3] = M[9] * M[14] - M[13] * M[10];
		Tmp[7] = M[8] * M[14] - M[12] * M[10];
		Tmp[11] = M[8] * M[13] - M[12] * M[9];

		Det[0] = M[5] * Tmp[0] - M[6] * Tmp[4] + M[7] * Tmp[8];
		Det[1] = M[4] * Tmp[1] - M[6] * Tmp[5] + M[7] * Tmp[9];
		Det[2] = M[4] * Tmp[2] - M[5] * Tmp[6] + M[7] * Tmp[10];
		Det[3] = M[4] * Tmp[3] - M[5] * Tmp[7] + M[6] * Tmp[11];

		const T Determinant = M[0] * Det[0] - M[1] * Det[1] + M[2] * Det[2] - M[3] * Det[3];
		const T	RDet = 1.0f / Determinant;

		Result.M[0] = RDet * Det[0];
		Result.M[4] = -RDet * Det[1];
		Result.M[8] = RDet * Det[2];
		Result.M[12] = -RDet * Det[3];
		Result.M[1] = -RDet * (M[1] * Tmp[0] - M[2] * Tmp[4] + M[3] * Tmp[8]);
		Result.M[5] = RDet * (M[0] * Tmp[1] - M[2] * Tmp[5] + M[3] * Tmp[9]);
		Result.M[9] = -RDet * (M[0] * Tmp[2] - M[1] * Tmp[6] + M[3] * Tmp[10]);
		Result.M[13] = RDet * (M[0] * Tmp[3] - M[1] * Tmp[7] + M[2] * Tmp[11]);
		Result.M[2] = RDet * (
			M[1] * (M[6] * M[15] - M[14] * M[7]) -
			M[2] * (M[5] * M[15] - M[13] * M[7]) +
			M[3] * (M[5] * M[14] - M[13] * M[6])
			);
		Result.M[6] = -RDet * (
			M[0] * (M[6] * M[15] - M[14] * M[7]) -
			M[2] * (M[4] * M[15] - M[12] * M[7]) +
			M[3] * (M[4] * M[14] - M[12] * M[6])
			);
		Result.M[10] = RDet * (
			M[0] * (M[5] * M[15] - M[13] * M[7]) -
			M[1] * (M[4] * M[15] - M[12] * M[7]) +
			M[3] * (M[4] * M[13] - M[12] * M[5])
			);
		Result.M[14] = -RDet * (
			M[0] * (M[5] * M[14] - M[13] * M[6]) -
			M[1] * (M[4] * M[14] - M[12] * M[6]) +
			M[2] * (M[4] * M[13] - M[12] * M[5])
			);
		Result.M[3] = -RDet * (
			M[1] * (M[6] * M[11] - M[10] * M[7]) -
			M[2] * (M[5] * M[11] - M[9] * M[7]) +
			M[3] * (M[5] * M[10] - M[9] * M[6])
			);
		Result.M[7] = RDet * (
			M[0] * (M[6] * M[11] - M[10] * M[7]) -
			M[2] * (M[4] * M[11] - M[8] * M[7]) +
			M[3] * (M[4] * M[10] - M[8] * M[6])
			);
		Result.M[11] = -RDet * (
			M[0] * (M[5] * M[11] - M[9] * M[7]) -
			M[1] * (M[4] * M[11] - M[8] * M[7]) +
			M[3] * (M[4] * M[9] - M[8] * M[5])
			);
		Result.M[15] = RDet * (
			M[0] * (M[5] * M[10] - M[9] * M[6]) -
			M[1] * (M[4] * M[10] - M[8] * M[6]) +
			M[2] * (M[4] * M[9] - M[8] * M[5])
			);
	}

	return Result;
}

template<typename T>
TVector4<T> TMatrix<T>::Row(int RowNum) const
{

	return TVector4<T>(M[RowNum * 4], M[RowNum * 4 + 1], M[RowNum * 4 + 2], M[RowNum * 4 + 3]);
}

template<typename T>
TVector4<T> TMatrix<T>::Column(int ColumnNum) const
{
	return TVector4<T>(M[ColumnNum], M[4 + ColumnNum], M[8 + ColumnNum], M[12 + ColumnNum]);
}

template<typename T>
TVector<T> TMatrix<T>::TransformPosition(const TVector<T>& V) const
{
	TVector4<T> Result = TransformVector4(TVector4<T>(V, 1.f));

	return TVector<T>(Result) / Result.W;
}

template<typename T>
TVector4<T> TMatrix<T>::TransformVector4(const TVector4<T>& V) const
{
	T X = V | Column(0);
	T Y = V | Column(1);
	T Z = V | Column(2);
	T W = V | Column(3);

	return TVector4<T>(X, Y, Z, W);
}

template<typename T>
TVector4<T> TMatrix<T>::InverseTransformVector4(const TVector4<T>& V) const
{
	TMatrix<T> InvSelf = Inverse();

	return InvSelf.TransformVector4(V);
}

template<typename T>
TMatrix<T> TMatrix<T>::Indentity()
{
	return TMatrix<T>({
		1.f, 0.f, 0.f,0.f,
		0.f, 1.f, 0.f,0.f,
		0.f, 0.f, 1.f,0.f,
		0.f, 0.f, 0.f,1.f });
}

template<typename T>
TMatrix<T> TMatrix<T>::Translate(TVector<T> Tranlation)
{
	return TMatrix<T>(
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			Tranlation.X, Tranlation.Y, Tranlation.Z, 1.0f
		});
}

template<typename T>
TMatrix<T> TMatrix<T>::RotateX(T Angle)
{
	T SinX = sin(Angle * PI / 180.f);
	T CosX = cos(Angle * PI / 180.f);
	return TMatrix<T>(
		{
			1.f, 0.f, 0.f,0.f,
			0.f, CosX, -SinX,0.f,
			0.f, SinX, CosX,0.f,
			0.f, 0.f, 0.f,1.f
		});
}

template<typename T>
TMatrix<T> TMatrix<T>::RotateY(T Angle)
{
	T SinY = sin(Angle * PI / 180.f);
	T CosY = cos(Angle * PI / 180.f);
	return TMatrix<T>(
		{
			CosY, 0.f, SinY,0.f,
			0.f, 1.f, 0.f,0.f,
			-SinY, 0.f, CosY,0.f,
			0.f, 0.f, 0.f,1.f
		});
}

template<typename T>
TMatrix<T> TMatrix<T>::RotateZ(T Angle)
{
	T SinZ = sin(Angle * PI / 180.f);
	T CosZ = cos(Angle * PI / 180.f);
	return TMatrix<T>(
		{
			CosZ, SinZ, 0.f,0.f,
			-SinZ, CosZ, 0.f,0.f,
			0.f, 0.f, 1.f,0.f,
			0.f, 0.f, 0.f,1.f
		});
}

template<typename T>
TMatrix<T> TMatrix<T>::Rotate(TVector<T> Rotation)
{
	return RotateZ(Rotation.Z) * RotateY(Rotation.Y) * RotateX(Rotation.X);
}

template<typename T>
TMatrix<T> TMatrix<T>::Scale(TVector<T> Scale3D)
{
	return TMatrix<T>(
		{
			Scale3D.X, 0.0f, 0.0f, 0.0f,
			0.0f, Scale3D.Y, 0.0f, 0.0f,
			0.0f, 0.0f, Scale3D.Z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		});
}

template<typename T>
TMatrix<T> TMatrix<T>::LookAt(TVector<T> Eye, TVector<T> Target, TVector<T> Up)
{
	TVector<T> AxisZ = Target.Normalize();
	TVector<T> AxisX = (Up ^ AxisZ).Normalize();
	TVector<T> AxisY = (AxisZ ^ AxisX).Normalize();

	T EyeX = - (AxisX | Eye);
	T EyeY = - (AxisY | Eye);
	T EyeZ = - (AxisZ | Eye);

	return TMatrix<T>(
		{
			AxisX.X, AxisY.X, AxisZ.X, 0.f,
			AxisX.Y, AxisY.Y, AxisZ.Y, 0.f,
			AxisX.Z, AxisY.Z, AxisZ.Z, 0.f,
			EyeX, EyeY, EyeZ, 1.f
		});
}

template<typename T>
TMatrix<T> TMatrix<T>::OrthoMatrix(T Width, T Height, T MinZ, T MaxZ)
{
	T ZScale = 1.f / (MaxZ - MinZ);
	T ZOffset = -MinZ;
	return TMatrix<T>(
		{
			(Width != 0.0f) ? (1.0f / Width) : 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, (Height != 0.0f) ? (1.0f / Height) : 1.f, 0.0f, 0.0f,
			0.0f, 0.0f, ZScale, 0.0f,
			0.0f, 0.0f, ZOffset * ZScale, 1.0f
		});
}

template<typename T>
TMatrix<T> TMatrix<T>::ReversedZOrthoMatrix(T Width, T Height, T MinZ, T MaxZ)
{
	T ZScale = 1.f / (MaxZ - MinZ);
	T ZOffset = -MinZ;
	return TMatrix<T>(
		{
			(Width != 0.0f) ? (1.0f / Width) : 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, (Height != 0.0f) ? (1.0f / Height) : 1.f, 0.0f, 0.0f,
			0.0f, 0.0f, -ZScale, 0.0f,
			0.0f, 0.0f, 1.0f - ZOffset * ZScale, 1.0f
		});
}

template<typename T>
TMatrix<T> TMatrix<T>::Perspective(T HalfFOV, T Width, T Height, T MinZ, T MaxZ)
{
	HalfFOV = HalfFOV * PI / 180.0f;
	return TMatrix<T>(
		{
			1.0f / (T)tan(HalfFOV), 0.0f, 0.0f, 0.0f,
			0.0f, Width / (T)tan(HalfFOV) / Height, 0.0f, 0.0f,
			0.0f, 0.0f, ((MinZ == MaxZ) ? 1.0f : MaxZ / (MaxZ - MinZ)), 1.0f,
			0.0f, 0.0f, ((MinZ == MaxZ) ? -MinZ : -MaxZ * MinZ / (MaxZ - MinZ)), 0.0f
		});
}

template<typename T>
TMatrix<T> TMatrix<T>::ReversedZPerspective(T HalfFOV, T Width, T Height, T MinZ, T MaxZ)
{
	HalfFOV = HalfFOV * PI / 180.0f;
	return TMatrix<T>(
		{
			1.0f / (T)tan(HalfFOV), 0.0f, 0.0f, 0.0f,
			0.0f, Width / (T)tan(HalfFOV) / Height, 0.0f, 0.0f,
			0.0f, 0.0f, ((MinZ == MaxZ) ? 0.0f : MinZ / (MinZ - MaxZ)), 1.0f,
			0.0f, 0.0f, ((MinZ == MaxZ) ? MinZ : -MaxZ * MinZ / (MinZ - MaxZ)), 0.0f
		});
}
