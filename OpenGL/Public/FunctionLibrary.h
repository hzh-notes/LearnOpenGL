#pragma once

#include <vector>
#include <string>

class FunctionLibrary
{
public:
	static std::string GetSolutionDir();

	static bool ReadImage(std::string Path, int* Width, int* Height, int* NumChannels, std::vector<unsigned char>& OutDatas);
};

