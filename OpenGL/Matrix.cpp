#include "Matrix.h"

float Matrix::operator[](const unsigned int Value)
{
	if (Value >= 16)
	{
		throw("out range");
	}

	return vecM[Value];
}
