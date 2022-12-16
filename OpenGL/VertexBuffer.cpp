#include "VertexBuffer.h"
#include "glad/glad.h"

#include <iostream>
using namespace std;

VertexBuffer::VertexBuffer(const VertexDescription* Vertices, int Size)
{
	//cout << Vertices << endl;

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBufferData(GL_ARRAY_BUFFER, Size, Vertices, GL_STATIC_DRAW);

	//unsigned int VAO;
	glGenVertexArrays(1, &BufferId);
	// 1. ��VAO
	glBindVertexArray(BufferId);
	// 2. �Ѷ������鸴�Ƶ�VAO�����й�OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Size, Vertices, GL_STATIC_DRAW);

	// 3. ���ö�������ָ��

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
