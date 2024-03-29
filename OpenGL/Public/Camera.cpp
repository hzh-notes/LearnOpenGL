#include "Camera.h"

Matrixf Camera::GetPerspectiveMatrix()
{
	return Matrixf::Perspective(fov / 2.0, witdh, height, nearClip, farClip);
}

Matrixf Camera::GetOrthoMatrix()
{
	return Matrixf::OrthoMatrix(witdh, height, nearClip, farClip);
}

Matrixf Camera::GetViewMatrix()
{
	const Vector3f eye = transform.Position;
	const Vector3f targrt = transform.GetMatrixWithScale().TransformVector4(Vector4f(1.f, 0.f, 0.f, 0.f));
	const Vector3f up = transform.GetMatrixWithScale().TransformVector4(Vector4f(0.f, 0.f, 1.f, 0.f));
	return Matrixf::LookAt(eye, targrt, up);
}
