#include "FunctionLibrary.h"
#include "stb_image.h"
#include <iostream>
#include <direct.h>
#include <windows.h>

std::string FunctionLibrary::GetSolutionDir()
{
	char Buffer[MAX_PATH];
	_getcwd(Buffer, MAX_PATH);
	std::string Dir = Buffer;

	return Dir;
}

bool FunctionLibrary::ReadImage(std::string Path, int* Width, int* Height, int* NumChannels, std::vector<unsigned char>& OutDatas)
{
	std::string ImagPath = GetSolutionDir() +"\\Texture\\" + Path;
	unsigned char* Src = stbi_load(ImagPath.c_str(), Width, Height, NumChannels, 0);
	OutDatas.resize((*Width) * (*Height) * (*NumChannels));
	memcpy(&OutDatas[0], Src, sizeof(char) * OutDatas.size());

	if (OutDatas.size() > 0)
	{
		stbi_image_free(Src);
		return true;
	}

	std::cout << "Failed to load texture" << std::endl;
	return false;
}
