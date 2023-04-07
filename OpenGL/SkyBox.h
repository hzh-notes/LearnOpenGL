#pragma once

#ifndef __SKYBOX_H
#define __SKYBOX_H

#include "Matrix.h"
#include <string>

class Mesh;
class ShaderProgram;

class SkyBox
{
public:

	SkyBox();
	~SkyBox();

	void Render(Matrix View, Matrix Projection);

private:
	int ShaderId = -1;
	Mesh* SkyMesh = nullptr;
};

#endif


