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
		0,1,2,0,2,3,		//1,3,0,1,2,3,//
		4,5,6,4,6,7,		//0,5,4,0,3,5,//
		8,9,10,8,10,11,		//7,0,4,7,1,0,//
		12,13,14,12,14,15,	//2,5,3,2,6,5,//
		16,17,18,16,18,19,	//7,4,5,7,5,6,//
		20,21,22,20,22,23,	//1,6,2,1,7,6,//
	};
}
