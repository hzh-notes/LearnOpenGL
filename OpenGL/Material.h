#pragma once

#ifndef __MATERIAL_H
#define __MATERIAL_H

#include <string>
#include <map>

enum class ETextureCategory
{
	Diffuse,
	Specular,
	Emission,
};

struct TextureParam
{
	int Id = -1;
	int Index = -1;
	std::string Path;
	ETextureCategory Category;
};

class Material
{
public:
	Material(int InID);

	bool Compile();

	void SetTextureParam(std::string Path, ETextureCategory Category);

	int GetTextureIdByCategory(ETextureCategory Category);

private:

	int BindImage(std::string ImagPath, int index);

public:
	bool bEmission = false;
	int shininess = 32;

private:
	int Id = 0;

	std::map<ETextureCategory, TextureParam> Textures;
};
#endif // !__MATERIAL_H


