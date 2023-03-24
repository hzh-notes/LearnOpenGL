#include "SkyBox.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Cube.h"

SkyBox::SkyBox()
{
	ShaderProgramMap::GetInstance()->AddShaderProgram("\\Shader\\SkyVS.glsl", "\\Shader\\SkyPS.glsl");
}

void SkyBox::Render(Matrix View, Matrix Projection)
{
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	ShaderProgram* SkyProgram = ShaderProgramMap::GetInstance()->GetByKey(0);

	SkyProgram->Use();

	Matrix ViewWithoutTranlate = View;
	ViewWithoutTranlate.M[12] = 0;
	ViewWithoutTranlate.M[13] = 0;
	ViewWithoutTranlate.M[14] = 0;
	SkyProgram->SetUniform4x4("view", ViewWithoutTranlate);
	SkyProgram->SetUniform4x4("projection", Projection);

	std::vector<MeshVertex> Vertices;
	std::vector<int> Indices;

	//GatherMeshInfo(SkyBox, Vertices, Indices, model);

	VertexBuffer* vBuffer = new VertexBuffer(Vertices.data(), sizeof(MeshVertex) * Vertices.size());
	IndexBuffer* iBuffer = new IndexBuffer(Indices.data(), sizeof(int) * Indices.size());

	SkyProgram->LoadSkyCubeMap();

	//绑定顶点和索引
	glBindVertexArray(vBuffer->BufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer->BufferId);
	//绘制模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//绘制
	//glDrawArrays(GL_TRIANGLES, 0, indices.size());
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	glDepthMask(GL_TRUE);
}

