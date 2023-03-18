#include "Shader.h"
#include "stb_image.h"

#include <iostream>
#include <direct.h>
#include <windows.h>
#include <fstream>
#include <sstream>

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

void ShaderProgram::SetUniform1i(const std::string& Name, int Value)
{
	int Location = glGetUniformLocation(ShaderProgramId, Name.c_str());
	glUniform1i(Location, Value);
}

void ShaderProgram::SetUniform4f(const std::string& Name, Vector4f Value)
{
	int Location = glGetUniformLocation(ShaderProgramId, Name.c_str());
	glUniform4f(Location, Value.X, Value.Y, Value.Z, Value.W);
}

void ShaderProgram::SetUniform3f(const std::string& Name, Vector3f Value)
{
	int Location = glGetUniformLocation(ShaderProgramId, Name.c_str());
	glUniform3f(Location, Value.X, Value.Y, Value.Z);
}

void ShaderProgram::SetUniform4x4(const std::string& Name, Matrix Value)
{
	int Location = glGetUniformLocation(ShaderProgramId, Name.c_str());
	glUniformMatrix4fv(Location, 1, GL_FALSE, Value.M);
}

void ShaderProgram::SetUniformTexture2D(const std::string& Name, std::string TextureName, int Index)
{
	char buffer[MAX_PATH];
	_getcwd(buffer, MAX_PATH);
	std::string path = buffer;

	unsigned int texture;
	std::string texturePath = path + "\\Texture\\" + TextureName;
	BindImage(texturePath, Index, texture);

	SetUniform1i(Name, Index);
}

int ShaderProgram::BindImage(std::string ImagPath, int index, unsigned int& OutTexture)
{
	glGenTextures(1, &OutTexture);

	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, OutTexture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int Width, Height, NumChannels;
	unsigned char* Data = stbi_load(ImagPath.c_str(), &Width, &Height, &NumChannels, 0);
	if (Data)
	{
		GLint TextureFormat = GL_RGB;
		if (NumChannels == 4)
		{
			TextureFormat = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, TextureFormat, Width, Height, 0, TextureFormat, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return -1;
	}

	stbi_image_free(Data);
	return 1;
}
