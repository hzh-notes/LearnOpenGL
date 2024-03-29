#include "SkyBox.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Shapes/Cube.h"
#include "Material.h"

SkyBox::SkyBox()
{
	ShaderId = ShaderProgramMap::GetInstance()->AddShaderProgram("\\Shader\\SkyVS.glsl", "\\Shader\\SkyPS.glsl");
	
	SkyMesh = new Cube(Transform());
	Mat = new Material();
	Mat->LoadCubeMap({ 
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg" });
	Mat->bSkyBox = true;
}

SkyBox::~SkyBox()
{
	delete SkyMesh, Mat;
	SkyMesh = nullptr;
	Mat = nullptr;
}

void SkyBox::Render(Matrixf View, Matrixf Projection)
{
	ShaderProgram* SkyProgram = ShaderProgramMap::GetInstance()->GetByKey(ShaderId);
	SkyProgram->Use();
	Mat->Compile(SkyProgram);
	Matrixf ViewWithoutTranlate = View;
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

	delete iBuffer, vBuffer;
	iBuffer = nullptr;
	vBuffer = nullptr;
}

