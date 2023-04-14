#pragma once

#ifndef __CAMERA
#define __CAMERA

#include "Math/Transform.h"
#include "Math/Matrix.h"

class Camera
{
public:

	Camera() {}

	Camera(Transform InTransform) :transform(InTransform) {}

	Matrix GetPerspectiveMatrix();

	Matrix GetOrthoMatrix();

	Matrix GetViewMatrix();

public:

	double nearClip = 1.0;
	double farClip = 10000.0;
	double fov = 90;
	double witdh = 1080.0;
	double height = 720.0;

	Transform transform;

};
#endif // !__CAMERA



