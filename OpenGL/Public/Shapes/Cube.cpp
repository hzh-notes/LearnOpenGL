#include "Cube.h"

void Cube::InitMesh()
{
	//Mesh::InitMesh();
	//positions =
	//{
	//	Vector3f(-50,-50,50), //0
	//	Vector3f(-50,-50,-50), //1
	//	Vector3f(50,-50,-50), //2
	//	Vector3f(50,-50,50), //3
	//	Vector3f(-50,50,50), //4
	//	Vector3f(50,50,50), //5
	//	Vector3f(50,50,-50), //6
	//	Vector3f(-50,50,-50), //7
	//};

	Vertices =
	{
		MeshVertex(Vector3f(50,-50,-50), Vector3f(0,-1,0), Vector4f(1,1,1,1), Vector2f(0,0)),
		MeshVertex(Vector3f(50,-50,50), Vector3f(0,-1,0), Vector4f(1,1,1,1), Vector2f(0,1)),
		MeshVertex(Vector3f(-50,-50,50), Vector3f(0,-1,0), Vector4f(1,1,1,1), Vector2f(1,1)),
		MeshVertex(Vector3f(-50,-50,-50), Vector3f(0,-1,0), Vector4f(1,1,1,1), Vector2f(1,0)),

		MeshVertex(Vector3f(-50,-50,50), Vector3f(0,0,1), Vector4f(1,1,1,1), Vector2f(0,0)),
		MeshVertex(Vector3f(50,-50,50), Vector3f(0,0,1), Vector4f(1,1,1,1), Vector2f(0,1)),
		MeshVertex(Vector3f(50,50,50), Vector3f(0,0,1), Vector4f(1,1,1,1), Vector2f(1,1)),
		MeshVertex(Vector3f(-50,50,50), Vector3f(0,0,1), Vector4f(1,1,1,1), Vector2f(1,0)),

		MeshVertex(Vector3f(-50,-50,-50), Vector3f(-1,0,0), Vector4f(1,1,1,1), Vector2f(0,0)),
		MeshVertex(Vector3f(-50,-50,50), Vector3f(-1,0,0), Vector4f(1,1,1,1), Vector2f(0,1)),
		MeshVertex(Vector3f(-50,50,50), Vector3f(-1,0,0), Vector4f(1,1,1,1), Vector2f(1,1)),
		MeshVertex(Vector3f(-50,50,-50), Vector3f(-1,0,0), Vector4f(1,1,1,1), Vector2f(1,0)),

		MeshVertex(Vector3f(50,50,-50), Vector3f(1,0,0), Vector4f(1,1,1,1), Vector2f(0,0)),
		MeshVertex(Vector3f(50,50,50), Vector3f(1,0,0), Vector4f(1,1,1,1), Vector2f(0,1)),
		MeshVertex(Vector3f(50,-50,50), Vector3f(1,0,0), Vector4f(1,1,1,1), Vector2f(1,1)),
		MeshVertex(Vector3f(50,-50,-50), Vector3f(1,0,0), Vector4f(1,1,1,1), Vector2f(1,0)),

		MeshVertex(Vector3f(-50,50,-50), Vector3f(0,1,0), Vector4f(1,1,1,1), Vector2f(0,0)),
		MeshVertex(Vector3f(-50,50,50), Vector3f(0,1,0), Vector4f(1,1,1,1), Vector2f(0,1)),
		MeshVertex(Vector3f(50,50,50), Vector3f(0,1,0), Vector4f(1,1,1,1), Vector2f(1,1)),
		MeshVertex(Vector3f(50,50,-50), Vector3f(0,1,0), Vector4f(1,1,1,1), Vector2f(1,0)),

		MeshVertex(Vector3f(-50,-50,-50), Vector3f(0,0,-1), Vector4f(1,1,1,1), Vector2f(0,0)),
		MeshVertex(Vector3f(50,-50,-50), Vector3f(0,0,-1), Vector4f(1,1,1,1), Vector2f(1,0)),
		MeshVertex(Vector3f(50,50,-50), Vector3f(0,0,-1), Vector4f(1,1,1,1), Vector2f(1,1)),
		MeshVertex(Vector3f(-50,50,-50), Vector3f(0,0,-1), Vector4f(1,1,1,1), Vector2f(0,1))
	};

	Indices =
	{
		0,2,1,0,3,2,
		4,6,5,4,7,6,
		8,10,9,8,11,10,
		12,14,13,12,15,14,
		16,18,17,16,19,18,
		20,21,22,20,22,23,
	};
}
