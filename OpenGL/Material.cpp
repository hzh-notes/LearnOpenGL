#include "Material.h"
#include "stb_image.h"
#include "glad/glad.h"
#include "Shader.h"
#include <iostream>
#include <direct.h>
#include <windows.h>

Material::Material(int InID)
{
	Id = InID;
	//±àÒë×ÅÉ«Æ÷
	ShaderId = ShaderProgramMap::GetInstance()->AddShaderProgram("\\Shader\\VertexShader.glsl", "\\Shader\\PixelShader.glsl");
}

bool Material::Compile()
{
	char buffer[MAX_PATH];
	_getcwd(buffer, MAX_PATH);
	std::string path = buffer;

	auto it = Textures.begin();
	for (int i = 0; i < Textures.size(); ++i, ++it)
	{
		auto& Texture = it->second;
		std::string texturePath = path + "\\Texture\\" + Texture.Path;
		BindImage(texturePath, Texture.Index);
		Texture.Id = Texture.Index;
	}

	return true;
}

void Material::SetTextureParam(std::string Path, ETextureCategory Category)
{
	TextureParam Param;
	Param.Index = Textures.size() + Id;
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

int Material::BindImage(std::string ImagPath, int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	unsigned int OutTexture;
	glGenTextures(1, &OutTexture);
	int Width, Height, NumChannels;
	unsigned char* Data = stbi_load(ImagPath.c_str(), &Width, &Height, &NumChannels, 0);
	if (Data)
	{
		GLint TextureFormat = GL_RGB;
		if (NumChannels == 4)
		{
			TextureFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, OutTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, TextureFormat, Width, Height, 0, TextureFormat, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return -1;
	}

	stbi_image_free(Data);
	return OutTexture;
}