#include "Plane.h"

void Plane::InitMesh()
{
	Vertices = {
		MeshVertex(Vector3f(-50.f, -50.f, 0.f), Vector3f(1.f, 0.f, 0.f), Vector4f(1.f, 0.f, 0.f, 1.f), Vector2f(0.f, 0.f)),
		MeshVertex(Vector3f(-50.f, 50.f, 0.f), Vector3f(1.f, 0.f, 0.f), Vector4f(1.f, 0.f, 0.f, 1.f), Vector2f(1.f, 0.f)),
		MeshVertex(Vector3f(50.f, 50.f, 0.f), Vector3f(1.f, 0.f, 0.f), Vector4f(1.f, 0.f, 0.f, 1.f), Vector2f(1.f, 1.f)),
		MeshVertex(Vector3f(50.f, -50.f, 0.f), Vector3f(1.f, 0.f, 0.f), Vector4f(1.f, 0.f, 0.f, 1.f), Vector2f(0.f, 1.f))
	};

	Indices = {
		0, 1, 2,
		0, 2, 3
	};
}
