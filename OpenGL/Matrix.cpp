#include "Matrix.h"

Matrix::Matrix()
{
	Matrix({
		0.f, 0.f, 0.f,0.f,
		0.f, 0.f, 0.f,0.f,
		0.f, 0.f, 0.f,0.f,
		0.f, 0.f, 0.f,0.f
		});
}

Matrix::Matrix(std::vector<float> values)
{
	for (int i = 0; i < 16; ++i)
	{
		m[i] = i < values.size() ? values[i] : 0.f;
	}
}

float Matrix::operator[](const unsigned int Value)
{
	if (Value >= 16)
	{
		throw("out range");
	}

	return m[Value];
}
