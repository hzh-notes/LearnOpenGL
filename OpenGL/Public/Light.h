#pragma once

#ifndef __LIGHT_H
#define __LIGHT_H

#include "Shapes/Mesh.h"
#include "Transform.h"

class Light
{
public:

	Light();

	void Render(std::vector<Mesh*> Meshes);

public:
	
	Transform LightTransform;

	Vector3f Ambient;
	Vector3f Diffuse;
	Vector3f Specular;

	int ShaderId;
};

#endif