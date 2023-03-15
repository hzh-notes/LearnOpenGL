#include "Mesh.h"

void Mesh::GetElementInfo(std::vector<MeshVertex>& OutVertices, std::vector<int>& OutIndices)
{
	int BaseVertexIndex = OutVertices.size();
	
	OutVertices.insert(OutVertices.end(), Vertices.begin(), Vertices.end());
	for (int i = 0; i < Indices.size(); ++i)
	{
		OutIndices.push_back(BaseVertexIndex + Indices[i]);
	}
}
