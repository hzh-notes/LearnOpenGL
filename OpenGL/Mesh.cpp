#include "Mesh.h"

void Mesh::GetElementInfo(std::vector<MeshVertex>& OutVertices, std::vector<int>& OutIndices)
{
	int BaseVertexIndex = OutVertices.size();

	for (auto Vertex : Vertices)
	{
		Vertex.Normal = MeshTransform.GetRotationMatrix().TransformVector4f(Vertex.Normal);
		OutVertices.push_back(Vertex);
	}

	for (int i = 0; i < Indices.size(); ++i)
	{
		OutIndices.push_back(BaseVertexIndex + Indices[i]);
	}
}

Material Mesh::GetMaterial()
{
	return Mat;
}

void Mesh::SetMaterial(Material InMaterial)
{
	Mat = InMaterial;
}
