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

	for (auto it = Textures.begin(); it != Textures.end(); ++it)
	{
		auto& Texture = it->second;

		int Index = (int)Texture.Category;
		glActiveTexture(GL_TEXTURE0 + Index);
		glBindTexture(GL_TEXTURE_2D, Texture.Id);
		switch (Texture.Category)
		{
		case ETextureCategory::Diffuse:
			Program->SetUniform1i("material.diffuse", 1);
			Program->SetUniformTexture2D("material.diffuseSampler", Index);
			break;
		case ETextureCategory::Specular:
			Program->SetUniform1i("material.specular", 1);
			Program->SetUniformTexture2D("material.specularSampler", Index);
			break;
		case ETextureCategory::Emission:
			Program->SetUniform1i("material.emission", 1);
			Program->SetUniformTexture2D("material.emissionSampler", Index);
			break;
		default:
			break;
		}

	}
	
	return true;
}

void Material::SetTextureParam(std::string Path, ETextureCategory Category)
{
	if (!Path.empty())
	{
		TextureParam Param;
		Param.Category = Category;
		if (BindImage(Path, Param))
		{
			Textures.emplace(Param.Category, Param);
		}
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

bool Material::BindImage(std::string Path, TextureParam& Texture)
{
	if (FunctionLibrary::ReadImage(Path, &Texture.Width, &Texture.Height, &Texture.NumChannels, Texture.Datas))
	{
		GLint TextureFormat = GL_RGB;
		if (Texture.NumChannels == 4)
		{
			TextureFormat = GL_RGBA;
		}

		glGenTextures(1, &Texture.Id);
		glBindTexture(GL_TEXTURE_2D, Texture.Id);
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
		
		return false;
	}

	return true;
}