#include "Light.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

Light::Light()
{
	ShaderId = ShaderProgramMap::GetInstance()->AddShaderProgram("\\Shader\\LightVS.glsl", "\\Shader\\LightPS.glsl");
}

Matrix Light::GetLightSpaceMatrix() const
{
	Matrix View = Matrix::LookAt(LightTransform.Position, LightTransform.GetForwardVector(), LightTransform.GetUpVector());//Vector3f(0, -0.866, -0.5), Vector3f(0, -0.5, 0.866));
	Matrix Projection = Matrix::Perspective(45.f, 1080.f, 720.f, 1.f, 10000.f);
	//Matrix::OrthoMatrix(1080.f, 720.f, 1.f, 100000.f);

	return View * Projection;
}

void Light::Render(std::vector<Mesh*> Meshes)
{
	ShaderProgram* Program = ShaderProgramMap::GetInstance()->GetByKey(ShaderId);
	Program->Use();
	

	for (Mesh* mesh : Meshes)
	{
		std::vector<MeshVertex> Vertices;
		std::vector<int> Indices;

		mesh->GetElementInfo(Vertices, Indices);

		VertexBuffer* vBuffer = new VertexBuffer(Vertices.data(), sizeof(MeshVertex) * Vertices.size());
		IndexBuffer* iBuffer = new IndexBuffer(Indices.data(), sizeof(int) * Indices.size());

		Matrix model = mesh->MeshTransform.GetMatrixWithScale();
		Program->SetUniform4x4("model", model);
		Program->SetUniform4x4("lightSpaceMatrix", GetLightSpaceMatrix());

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
}
