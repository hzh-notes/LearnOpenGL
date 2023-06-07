#pragma once

#ifndef __MATERIAL_H
#define __MATERIAL_H

#include <vector>
#include <string>
#include <map>

enum class ETextureCategory : unsigned short
{
	Depth = 0,
	Diffuse = 0x01,
	Specular = 0x02,
	Emission = 0x03,

	CubeMap = 0x0A,
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

	void LoadCubeMap(std::vector<std::string> Paths);

	int GetTextureIdByCategory(ETextureCategory Category);

private:

	bool BindImage(std::string Path, TextureParam& Texture);

public:
	int shininess = 32;

	bool bSkyBox = false;
private:

	std::map<ETextureCategory, TextureParam> Textures;
};
#endif // !__MATERIAL_H


