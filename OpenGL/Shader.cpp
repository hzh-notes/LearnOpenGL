#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* ShaderPath, EShaderType ShaderType)
{
	this->ShaderType = ShaderType;

	std::ifstream ShaderFile{};

	ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		ShaderFile.open(ShaderPath);
		std::stringstream ShaderStream{};
		ShaderStream << ShaderFile.rdbuf();

		ShaderFile.close();

		ShaderCode = ShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

}

int Shader::Compile()
{
	const char* ShaderSource = ShaderCode.c_str();

	this->ShaderId = glCreateShader(GLenum(ShaderType));
	glShaderSource(this->ShaderId, 1, &ShaderSource, NULL);
	glCompileShader(this->ShaderId);

	int  Success;
	char InfoLog[512];
	glGetShaderiv(this->ShaderId, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(this->ShaderId, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::" << GLenum(ShaderType) << "::COMPILATION_FAILED\n" << InfoLog << std::endl;
	}

	return Success;
}

ShaderProgram::ShaderProgram(unsigned int VertexShaderId, unsigned int PixelShaderId)
{
	ShaderProgramId = glCreateProgram();
	glAttachShader(ShaderProgramId, VertexShaderId);
	glAttachShader(ShaderProgramId, PixelShaderId);
	glLinkProgram(ShaderProgramId);
}

int ShaderProgram::Compile()
{
	int  success;
	char infoLog[512];
	glGetProgramiv(ShaderProgramId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ShaderProgramId, 512, NULL, infoLog);
	}

	return success;
}

void ShaderProgram::Use()
{
	glUseProgram(ShaderProgramId);
}

void ShaderProgram::SetUniform4F(const std::string& Name, Vector4f Value)
{
	int vertexColorLocation = glGetUniformLocation(ShaderProgramId, Name.c_str());
	glUniform4f(vertexColorLocation, Value.X, Value.Y, Value.Z, Value.W);
}
