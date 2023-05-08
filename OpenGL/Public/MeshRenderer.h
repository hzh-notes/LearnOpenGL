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

	void Render(std::vector<Mesh*> Meshes, const Matrix& View, const Matrix& Projection, const Vector3f& ViewPos, const unsigned int& DepthTexture, const Matrix& LightSpace);

private:

	void GatherMeshInfo(Mesh* InMesh, std::vector<MeshVertex>& OutVertices, std::vector<int>& OutIndices, Matrix& OutModel);
	
private:

	int ShaderId = -1;
};

#endif // !__MESHRENDERER_H


