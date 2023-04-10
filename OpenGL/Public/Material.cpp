#include "Material.h"
#include "glad/glad.h"
#include "Shader.h"
#include "FunctionLibrary.h"

Material::Material()
{

}

bool Material::Compile(ShaderProgram* Program)
{
	Program->SetUniform1i("material.diffuse", 0);
	Program->SetUniform1i("material.specular", 0);
	Program->SetUniform1i("material.emission", 0);
	Program->SetUniform1i("material.shininess", shininess);

	auto it = Textures.begin();
	for (int i = 0; i < Textures.size(); ++i, ++it)
	{
		auto& Texture = it->second;
		BindImage(Texture);

		int value = (int)Texture.Category;
		switch (Texture.Category)
		{
		case ETextureCategory::Diffuse:
			Program->SetUniform1i("material.diffuse", 1);
			Program->SetUniformTexture2D("material.diffuseSampler", value);
			break;
		case ETextureCategory::Specular:
			Program->SetUniform1i("material.specular", 1);
			Program->SetUniformTexture2D("material.specularSampler", value);
			break;
		case ETextureCategory::Emission:
			Program->SetUniform1i("material.emission", 1);
			Program->SetUniformTexture2D("material.emissionSampler", value);
			break;
		default:
			break;
		}

	}
	
	return true;
}

void Material::SetTextureParam(std::string Path, ETextureCategory Category)
{
	int Width, Height, NumChannels;
	std::vector<unsigned char> Datas;

	if (FunctionLibrary::ReadImage(Path, &Width, &Height, &NumChannels, Datas))
	{
		TextureParam Param;
		Param.Id = Textures.size() + 1;
		Param.Width = Width;
		Param.Height = Height;
		Param.NumChannels = NumChannels;
		Param.Datas = Datas;
		Param.Category = Category;
		Textures.emplace(Param.Category, Param);
	}
	
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

int Material::BindImage(TextureParam Texture)
{
	glActiveTexture(GL_TEXTURE0 + (GLenum)Texture.Category);
	unsigned int TextureId;
	glGenTextures(1, &TextureId);
	if (Texture.Datas.size() > 0)
	{
		GLint TextureFormat = GL_RGB;
		if (Texture.NumChannels == 4)
		{
			TextureFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, TextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, TextureFormat, Texture.Width, Texture.Height, 0, TextureFormat, GL_UNSIGNED_BYTE, Texture.Datas.data());
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
		
		return -1;
	}

	return TextureId;
}