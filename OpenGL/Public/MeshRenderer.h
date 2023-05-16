#pragma once

#ifndef __MESHRENDERER_H
#define __MESHRENDERER_H

#include "Shapes/Mesh.h"
#include "Math/Matrix.h"
#include <vector>
class MeshRenderer
{
public:

	MeshRenderer();

	void Render(std::vector<Mesh*> Meshes, const Matrixf& View, const Matrixf& Projection, const Vector3f& ViewPos, const Matrixf& LightSpace);

private:

	void GatherMeshInfo(Mesh* InMesh, std::vector<MeshVertex>& OutVertices, std::vector<int>& OutIndices, Matrixf& OutModel);
	
private:

	int ShaderId = -1;
};

#endif // !__MESHRENDERER_H


