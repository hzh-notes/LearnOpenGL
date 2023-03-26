#include "SkyBox.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Cube.h"

SkyBox::SkyBox()
{
	ShaderProgramMap::GetInstance()->AddShaderProgram(1, "\\Shader\\SkyVS.glsl", "\\Shader\\SkyPS.glsl");
	ShaderProgramMap::GetInstance()->GetByKey(1)->LoadSkyCubeMap();

	SkyMesh = new Cube(Transform());
}

SkyBox::~SkyBox()
{
	delete SkyMesh;
	SkyMesh = nullptr;
}

void SkyBox::Render(Matrix View, Matrix Projection)
{
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	ShaderProgram* SkyProgram = ShaderProgramMap::GetInstance()->GetByKey(1);
	SkyProgram->Use();

	Matrix ViewWithoutTranlate = View;
	ViewWithoutTranlate.M[12] = 0;
	ViewWithoutTranlate.M[13] = 0;
	ViewWithoutTranlate.M[14] = 0;

	SkyProgram->SetUniform4x4("view", ViewWithoutTranlate);
	SkyProgram->SetUniform4x4("projection", Projection);

	std::vector<MeshVertex> Vertices;
	std::vector<int> Indices;

	Vertices = SkyMesh->Vertices;
	Indices = SkyMesh->Indices;

	VertexBuffer* vBuffer = new VertexBuffer(Vertices.data(), sizeof(MeshVertex) * Vertices.size());
	IndexBuffer* iBuffer = new IndexBuffer(Indices.data(), sizeof(int) * Indices.size());

	//绑定顶点和索引
	glBindVertexArray(vBuffer->BufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer->BufferId);
	//绘制模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//绘制
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	glDepthMask(GL_TRUE);
}

