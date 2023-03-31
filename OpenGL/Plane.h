#pragma once

#ifndef __PLANE_H
#define __PLANE_H

#include "Mesh.h"

class Plane : public Mesh
{
public:

	Plane() : Mesh()
	{
		InitMesh();
	}

	Plane(Transform InTransform) : Mesh(InTransform)
	{
		InitMesh();
	}

protected:

	virtual void InitMesh() override;
};
#endif


