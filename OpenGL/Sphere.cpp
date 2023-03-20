#include "Sphere.h"

constexpr double PI = 3.1415926535;

#define DegreeToRadian(X) X * PI / 180.

void Sphere::InitMesh()
{
	const float Radius = 100.f;
	const int Segement = 24;
	const float SegDegree = 360. / 24;

	for (int i = 0; i < Segement / 2; ++i)
	{
		for (int j = 0; j < Segement; ++j)
		{
			int BaseNumVertices = Vertices.size();
			Indices.push_back(BaseNumVertices + 0);
			Indices.push_back(BaseNumVertices + 1);
			Indices.push_back(BaseNumVertices + 2);
			Indices.push_back(BaseNumVertices + 0);
			Indices.push_back(BaseNumVertices + 2);
			Indices.push_back(BaseNumVertices + 3);

			Vector2f UVs[4] = { Vector2f(j, i), Vector2f(j + 1, i), Vector2f(j + 1, i + 1), Vector2f(j, i + 1) };

			for (int index = 0; index < 4; ++index)
			{
				Vector2f UV = UVs[index];
				float AdjustRadius = sin(DegreeToRadian(UV.Y * SegDegree)) * Radius;
				float Z = cos(DegreeToRadian(UV.Y * SegDegree)) * Radius;
				float Y = sin(DegreeToRadian(UV.X * SegDegree)) * AdjustRadius;
				float X = cos(DegreeToRadian(UV.X * SegDegree)) * AdjustRadius;

				MeshVertex Vertex;
				Vertex.Position = Vector3f(X, Y, Z);
				Vertex.Normal = Vertex.Position.Normalize();
				Vertex.Color = Vector4f(1.0);
				Vertex.UV = UV / Vector2f(Segement, Segement / 2.);
				Vertex.UV = Vector2f(1.) - Vertex.UV;

				Vertices.push_back(Vertex);
			}
		}
	}
}
