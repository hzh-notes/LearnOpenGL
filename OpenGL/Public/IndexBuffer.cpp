#include "IndexBuffer.h"
#include "glad/glad.h"

IndexBuffer::IndexBuffer(const int* Indices, int Size)
{
	NumBytes = Size;

	glGenBuffers(1, &BufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumBytes, Indices, GL_STATIC_DRAW);
}

int IndexBuffer::GetNumIndices()
{
	return NumBytes  / sizeof(int);
}
