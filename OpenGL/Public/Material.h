#pragma once

#ifndef __MATERIAL_H
#define __MATERIAL_H

#include <vector>
#include <string>
#include <map>

enum class ETextureCategory
{
	Diffuse = 0,
	Specular = 1,
	Emission = 2,
};

struct TextureParam
{
	unsigned int Id = 0;
	int Width = 0;
	int Height = 0;
	int NumChannels = 0;
	ETextureCategory Category;
	std::vector<unsigned char> Datas;
};

class ShaderProgram;

class Material
{
public:
	Material();

	bool Compile(ShaderProgram* Program);

	void SetTextureParam(std::string Path, ETextureCategory Category);

	int GetTextureIdByCategory(ETextureCategory Category);

private:

	bool BindImage(std::string Path, TextureParam& Texture);

public:
	int shininess = 32;
private:

	std::map<ETextureCategory, TextureParam> Textures;
};
#endif // !__MATERIAL_H


