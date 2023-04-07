#pragma once

#ifndef __SHADER_H
#define __SHADER_H

#include "glad/glad.h"
#include <string>
#include <map>
#include "Vector.h"
#include "Matrix.h"

enum class EShaderType : GLenum
{
	VertexShader = GL_VERTEX_SHADER,
	PixelShader = GL_FRAGMENT_SHADER,
	GeometryShader = GL_GEOMETRY_SHADER,
	None = 0
};

class Shader
{
public:

	Shader(const char* ShaderPath, EShaderType ShaderType);
	~Shader() {};

	int Compile();

public:

	unsigned int ShaderId = -1;

	EShaderType ShaderType = EShaderType::None;

	std::string ShaderCode{};
};

class ShaderProgram
{
public:
	ShaderProgram(unsigned int VertexShaderId, unsigned int PixelShaderId);
	~ShaderProgram() {};

	int Compile();

	void Use();

	void LoadSkyCubeMap();

	void SetUniform1i(const std::string& Name, int Value);

	void SetUniform1f(const std::string& Name, float Value);

	void SetUniform2f(const std::string& Name, Vector2f Value);

	void SetUniform3f(const std::string& Name, Vector3f Value);

	void SetUniform4f(const std::string& Name, Vector4f Value);

	void SetUniform4x4(const std::string& Name, Matrix Value);

	void SetUniformTexture2D(const std::string& Name, int Index);

private:

public:

	unsigned int ShaderProgramId = -1;
};

class ShaderProgramMap
{
private:
	ShaderProgramMap() {}

public:

	static ShaderProgramMap* GetInstance();

	int AddShaderProgram(std::string VS, std::string PS);

	void RemoveShaderProgram(int Index);

	ShaderProgram* GetByKey(int Key);

private:

	std::map<int, ShaderProgram*> Programs;

};

static ShaderProgramMap* _Instance = nullptr;

#endif