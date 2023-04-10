#pragma once

#ifndef __INDEXBUFFER
#define __INDEXBUFFER

class IndexBuffer
{
public:

	IndexBuffer(const int* Indices, int Size);

	int GetNumIndices();

public:

	unsigned int BufferId = -1;

private:

	int NumBytes = 0;
};

#endif


