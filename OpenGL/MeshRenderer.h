#pragma once

#ifndef __MESHRENDERER_H
#define __MESHRENDERER_H

#include "Mesh.h"
#include "Matrix.h"
#include <vector>
class MeshRenderer
{
public:

	MeshRenderer();

	void Render(std::vector<Mesh*> Meshes, const Matrix& View, const Matrix& Projection, const Vector3f& ViewPos);

private:

	void GatherMeshInfo(Mesh* InMesh, std::vector<MeshVertex>& OutVertices, std::vector<int>& OutIndices, Matrix& OutModel);
	
};

#endif // !__MESHRENDERER_H


