#include "VertexBuffer.h"
#include "glad/glad.h"

#include <iostream>
using namespace std;

VertexBuffer::VertexBuffer(const MeshVertex* Vertices, int Size)
{
	//cout << Vertices << endl;

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBufferData(GL_ARRAY_BUFFER, Size, Vertices, GL_STATIC_DRAW);

	//unsigned int VAO;
	glGenVertexArrays(1, &BufferId);
	// 1. 绑定VAO
	glBindVertexArray(BufferId);
	// 2. 把顶点数组复制到VAO缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Size, Vertices, GL_STATIC_DRAW);

	// 3. 设置顶点属性指针
	unsigned int Stride = sizeof(MeshVertex);
	SetVertexElement(0, 3, GL_FLOAT, Stride, offsetof(MeshVertex, Position), false);
	SetVertexElement(1, 3, GL_FLOAT, Stride, offsetof(MeshVertex, Normal), false);
	SetVertexElement(2, 4, GL_FLOAT, Stride, offsetof(MeshVertex, Color), false);
	SetVertexElement(3, 2, GL_FLOAT, Stride, offsetof(MeshVertex, UV), false);

	/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);*/
}

void VertexBuffer::SetVertexElement(unsigned int Index, unsigned int Size, unsigned int Type, unsigned int Stride, unsigned int Offset, bool Normalized)
{
	glVertexAttribPointer(Index, Size, Type, Normalized, Stride, (void*)Offset);
	glEnableVertexAttribArray(Index);
}
