#pragma once

#ifndef __SPHERE_H
#define __SPHERE_H

#include "Mesh.h"

class Sphere : public Mesh
{
public:

	Sphere() :Mesh()
	{
		InitMesh();
	}

	Sphere(Transform InTransform) :Mesh(InTransform)
	{
		InitMesh();
	}

protected:

	virtual void InitMesh() override;
};

#endif


