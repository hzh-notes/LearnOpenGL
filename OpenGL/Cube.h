#pragma once

#include "Mesh.h"

class Cube : public Mesh
{
public:

	Cube() :Mesh() 
	{
		InitMesh();
	}

	Cube(Transform InTransform) :Mesh(InTransform) 
	{
		InitMesh();
	}

protected:

	virtual void InitMesh() override;
};

