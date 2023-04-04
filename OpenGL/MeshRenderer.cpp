#include "MeshRenderer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

MeshRenderer::MeshRenderer()
{
	//编译着色器
	ShaderProgramMap::GetInstance()->AddShaderProgram(1, "\\Shader\\VertexShader.glsl", "\\Shader\\PixelShader.glsl");

}

void MeshRenderer::Render(std::vector<Mesh*> Meshes, const Matrix& View, const Matrix& Projection, const Vector3f& ViewPos)
{
	glFrontFace(GL_CCW);
	ShaderProgram* Program = ShaderProgramMap::GetInstance()->GetByKey(1);

	if (Program)
	{
		Program->Use();

		Program->SetUniform4x4("view", View);
		Program->SetUniform4x4("projection", Projection);
		Program->SetUniform3f("viewPos", ViewPos);

		//light
		Program->SetUniform3f("spotLight.position", Vector3f(0.f, 0.f, 100.f));
		Program->SetUniform3f("spotLight.direction", Vector3f(0.86, 0, -0.5));
		Program->SetUniform3f("spotLight.ambient", Vector3f(0.1f));
		Program->SetUniform3f("spotLight.diffuse", Vector3f(0.5f));
		Program->SetUniform3f("spotLight.specular", Vector3f(1.f));
		Program->SetUniform1f("spotLight.constant", 1.f);
		Program->SetUniform1f("spotLight.linear", 0.0014f);
		Program->SetUniform1f("spotLight.quadratic", 0.000007f);
		Program->SetUniform1f("spotLight.innerCutOff", cos(3.1415926f / 24.f));
		Program->SetUniform1f("spotLight.outerCutOff", cos(3.1415926f / 12.f));

		Program->SetUniform3f("dirLight.direction", Vector3f(0.f, -1.f, 0.f));
		Program->SetUniform3f("dirLight.ambient", Vector3f(0.1f));
		Program->SetUniform3f("dirLight.diffuse", Vector3f(0.5f));
		Program->SetUniform3f("dirLight.specular", Vector3f(1.f));

		for (Mesh* mesh : Meshes)
		{
			std::vector<MeshVertex> Vertices;
			std::vector<int> Indices;

			Matrix model;
			GatherMeshInfo(mesh, Vertices, Indices, model);

			VertexBuffer* vBuffer = new VertexBuffer(Vertices.data(), sizeof(MeshVertex) * Vertices.size());
			IndexBuffer* iBuffer = new IndexBuffer(Indices.data(), sizeof(int) * Indices.size());
			Material* mat = mesh->GetMaterial();

			Program->SetUniform4x4("model", model);

			//material
			int DiffuseId = mat->GetTextureIdByCategory(ETextureCategory::Diffuse);
			if (DiffuseId != -1)
			{
				Program->SetUniform1i("material.diffuse", 1);
				Program->SetUniformTexture2D("material.diffuseSampler", DiffuseId);
			}
			else
			{
				Program->SetUniform1i("material.diffuse", 0);
			}

			int SpecularId = mat->GetTextureIdByCategory(ETextureCategory::Specular);
			if (SpecularId != -1)
			{
				Program->SetUniform1i("material.specular", 1);
				Program->SetUniformTexture2D("material.specularSampler", SpecularId);
			}
			else
			{
				Program->SetUniform1i("material.specular", 0);
			}

			int EmissionId = mat->GetTextureIdByCategory(ETextureCategory::Emission);
			if (EmissionId != -1)
			{
				Program->SetUniform1i("material.emission", 1);
				Program->SetUniformTexture2D("material.emissionSampler", EmissionId);
			}
			else
			{
				Program->SetUniform1i("material.emission", 0);
			}

			Program->SetUniform1i("material.shininess", mat->shininess);

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
}

void MeshRenderer::GatherMeshInfo(Mesh* InMesh, std::vector<MeshVertex>& OutVertices, std::vector<int>& OutIndices, Matrix& OutModel)
{
	OutModel = InMesh->MeshTransform.GetMatrixWithScale();
	InMesh->GetElementInfo(OutVertices, OutIndices);
}
