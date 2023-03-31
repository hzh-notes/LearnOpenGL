#include "Material.h"
#include "stb_image.h"
#include "glad/glad.h"
#include <iostream>
#include <direct.h>
#include <windows.h>

Material::Material(int InID)
{
	Id = InID;
}

bool Material::Compile()
{
	char buffer[MAX_PATH];
	_getcwd(buffer, MAX_PATH);
	std::string path = buffer;

	for (auto Texture : Textures)
	{
		unsigned int texture;
		std::string texturePath = path + "\\Texture\\" + Texture.second.Path;
		BindImage(texturePath, Texture.second.Id, texture);
	}

	return true;
}

void Material::SetTextureParam(std::string Path, ETextureCategory Category)
{
	TextureParam Param;
	Param.Id = Textures.size() + Id * 3;
	Param.Path = Path;
	Param.Category = Category;
	Textures.emplace(Param.Category, Param);
}

int Material::GetTextureIdByCategory(ETextureCategory Category)
{
	auto it = Textures.find(Category);
	if (it == Textures.end())
	{
		return -1;
	}
	return it->second.Id;
}

int Material::BindImage(std::string ImagPath, int index, unsigned int& OutTexture)
{
	glGenTextures(1, &OutTexture);

	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, OutTexture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int Width, Height, NumChannels;
	unsigned char* Data = stbi_load(ImagPath.c_str(), &Width, &Height, &NumChannels, 0);
	if (Data)
	{
		GLint TextureFormat = GL_RGB;
		if (NumChannels == 4)
		{
			TextureFormat = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, TextureFormat, Width, Height, 0, TextureFormat, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return -1;
	}

	stbi_image_free(Data);
	return 1;
}