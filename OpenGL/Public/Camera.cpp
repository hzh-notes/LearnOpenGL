#include "Camera.h"

Matrix Camera::GetPerspectiveMatrix()
{
	return Matrix::Perspective(fov / 2.0, witdh, height, nearClip, farClip);
}

Matrix Camera::GetOrthoMatrix()
{
	return Matrix::OrthoMatrix(witdh, height, nearClip, farClip);
}

Matrix Camera::GetViewMatrix()
{
	const Vector3f eye = transform.Position;
	const Vector3f targrt = transform.GetMatrixWithScale().TransformVector4f(Vector4f(1.f, 0.f, 0.f, 0.f));
	const Vector3f up = transform.GetMatrixWithScale().TransformVector4f(Vector4f(0.f, 0.f, 1.f, 0.f));
	return Matrix::LookAt(eye, targrt, up);
}
