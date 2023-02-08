#include "Mesh.h"

void Mesh::GetElementInfo(std::vector<VertexDescription>& OutVertices, std::vector<int>& OutIndices)
{
	Matrix transformMat = transform.GetMatrixWithScale();
	for(int i = 0; i < positions.size(); ++i)
	{
		Vector3f worldPosition = transformMat.TransformPosition(positions[i]);
		Vector2f uv = uvs[i];
		OutVertices.push_back(VertexDescription(worldPosition, Vector4f(1), uv));
	}

	OutIndices = indices;
}
