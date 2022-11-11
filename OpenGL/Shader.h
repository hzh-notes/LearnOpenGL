#pragma once

#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include <string>
#include "Vector3.h"
#include "Vector4.h"

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

	void SetUniform4F(const std::string& Name, Vector4f Value);

private:

	unsigned int ShaderProgramId = -1;
};


#endif