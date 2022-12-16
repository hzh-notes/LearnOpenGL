#pragma once

#ifndef __MATRIX
#define __MATRIX

#include <vector>

struct Matrix
{
public:

	Matrix();

	Matrix(std::vector<float> values);

	//Matrix operator+(const float Value);

	float operator[](const unsigned int Value);

public:
	//static Matrix* Indentity = new Matrix({});

private:
	float m[16];
};

#endif


