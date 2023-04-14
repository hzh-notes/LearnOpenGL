#include "Light.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

Light::Light()
{
	ShaderId = ShaderProgramMap::GetInstance()->AddShaderProgram("\\Shader\\LightVS.glsl", "\\Shader\\LightPS.glsl");
}

void Light::Render(std::vector<Mesh*> Meshes)
{
	ShaderProgram* Program = ShaderProgramMap::GetInstance()->GetByKey(ShaderId);
	Program->Use();
	Matrix View = Matrix::LookAt(Vector3f(0), Vector3f(0, -0.866, -0.5), Vector3f(0, -0.5, 0.866));
	Matrix Projection = Matrix::OrthoMatrix(1080.f, 720.f, 1.f, 1000000.f);

	for (Mesh* mesh : Meshes)
	{
		std::vector<MeshVertex> Vertices;
		std::vector<int> Indices;

		VertexBuffer* vBuffer = new VertexBuffer(Vertices.data(), sizeof(MeshVertex) * Vertices.size());
		IndexBuffer* iBuffer = new IndexBuffer(Indices.data(), sizeof(int) * Indices.size());
		Material* mat = mesh->GetMaterial();

		Program->SetUniform4x4("view", View);
		Program->SetUniform4x4("projection", Projection);

		mat->Compile(Program);

		//绑定顶点和索引
		glBindVertexArray(vBuffer->BufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer->BufferId);
		//绘制模式
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//绘制
		//glDrawArrays(GL_TRIANGLES, 0, indices.size());
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

		delete iBuffer, vBuffer;
		iBuffer = nullptr;
		vBuffer = nullptr;
	}
}
