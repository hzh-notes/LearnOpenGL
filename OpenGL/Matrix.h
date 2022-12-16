#pragma once

#ifndef __MATRIX
#define __MATRIX

#include <vector>

class Matrix
{
public:

	Matrix() : vecM({ 1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0 })
	{
		m[0] = 1.0f,
			m[1] = 0.0f,
			m[2] = 0.0f,
			m[3] = 0.0f;
		m[4] = 0.0f,
			m[5] = 1.0f,
			m[6] = 0.0f,
			m[7] = 0.0f;
		m[8] = 0.0f,
			m[9] = 0.0f,
			m[10] = 1.0f,
			m[11] = 0.0f;
		m[12] = 0.0f,
			m[13] = 0.0f,
			m[14] = 0.0f,
			m[15] = 1.0f;

		M = &m[0];
	}

	//Matrix operator+(const float Value);

	float operator[](const unsigned int Value);

private:
	float m[16];
	std::vector<float> vecM;

	float* M;
};

#endif


