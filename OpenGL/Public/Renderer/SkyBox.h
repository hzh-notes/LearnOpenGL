#pragma once

#ifndef __SKYBOX_H
#define __SKYBOX_H

#include "Math/Matrix.h"
#include <string>

class Mesh;
class ShaderProgram;
class Material;

class SkyBox
{
public:

	SkyBox();
	~SkyBox();

	void Render(Matrixf View, Matrixf Projection);

private:
	int ShaderId = -1;
	Mesh* SkyMesh = nullptr;
	Material* Mat = nullptr;
};

#endif


