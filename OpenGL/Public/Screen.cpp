#include "Screen.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

Screen::Screen()
{
	ShaderId = ShaderProgramMap::GetInstance()->AddShaderProgram("\\Shader\\ViewportVS.glsl", "\\Shader\\ViewportPS.glsl");
}

Screen::~Screen()
{

}

void Screen::Render(unsigned int ColorBuffer)
{
	ShaderProgram* Program = ShaderProgramMap::GetInstance()->GetByKey(ShaderId);

	if (Program)
	{
		Program->Use();
		Program->SetUniformTexture2D("screenTexture", 0);

		//绑定顶点和索引
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
		};
		unsigned int quadVAO, quadVBO;
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//绘制
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}
}
