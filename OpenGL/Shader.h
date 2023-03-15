#pragma once

#ifndef __SHADER_H
#define __SHADER_H

#include "glad/glad.h"
#include <string>
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

	void SetUniform1i(const std::string& Name, int Value);

	void SetUniform4f(const std::string& Name, Vector4f Value);

	void SetUniform3f(const std::string& Name, Vector3f Value);

	void SetUniform4x4(const std::string& Name, Matrix Value);

private:

	unsigned int ShaderProgramId = -1;
};


#endif