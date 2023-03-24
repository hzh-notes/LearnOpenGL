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

void ShaderProgram::LoadSkyCubeMap()
{
	std::string Faces[6] = {
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	char buffer[MAX_PATH];
	_getcwd(buffer, MAX_PATH);
	std::string path = buffer;

	int Width, Height, NumChannels;
	unsigned char* Data;
	for (unsigned int i = 0; i < 6; i++)
	{
		std::string filePath = path + "\\Texture\\skybox\\" + Faces[i];
		Data = stbi_load(filePath.c_str(), &Width, &Height, &NumChannels, 0);
		if (Data)
		{
			GLint TextureFormat = GL_RGB;
			if (NumChannels == 4)
			{
				TextureFormat = GL_RGBA;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, TextureFormat, Width, Height, 0, TextureFormat, GL_UNSIGNED_BYTE, Data);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(Data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	SetUniform1i("skybox", 0);
}

void ShaderProgram::SetUniform1i(const std::string& Name, int Value)
{
	int Location = glGetUniformLocation(ShaderProgramId, Name.c_str());
	glUniform1i(Location, Value);
}

void ShaderProgram::SetUniform1f(const std::string& Name, float Value)
{
	int Location = glGetUniformLocation(ShaderProgramId, Name.c_str());
	glUniform1f(Location, Value);
}

void ShaderProgram::SetUniform2f(const std::string& Name, Vector2f Value)
{
	int Location = glGetUniformLocation(ShaderProgramId, Name.c_str());
	glUniform2f(Location, Value.X, Value.Y);
}

void ShaderProgram::SetUniform3f(const std::string& Name, Vector3f Value)
{
	int Location = glGetUniformLocation(ShaderProgramId, Name.c_str());
	glUniform3f(Location, Value.X, Value.Y, Value.Z);
}

void ShaderProgram::SetUniform4f(const std::string& Name, Vector4f Value)
{
	int Location = glGetUniformLocation(ShaderProgramId, Name.c_str());
	glUniform4f(Location, Value.X, Value.Y, Value.Z, Value.W);
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

ShaderProgramMap* ShaderProgramMap::GetInstance()
{
	if (_Instance == nullptr)
	{
		_Instance = new ShaderProgramMap();
	}

	return _Instance;
}

void ShaderProgramMap::AddShaderProgram(std::string VS, std::string PS)
{
	char buffer[MAX_PATH];
	_getcwd(buffer, MAX_PATH);
	std::string path = buffer;

	std::string ShaderVS = path + VS;
	std::string ShaderPS = path + PS;

	Shader* VertexShader = new Shader(ShaderVS.c_str(), EShaderType::VertexShader);
	Shader* PixelShader = new Shader(ShaderPS.c_str(), EShaderType::PixelShader);

	if (VertexShader->Compile() && PixelShader->Compile())
	{
		ShaderProgram* Program = new ShaderProgram(VertexShader->ShaderId, PixelShader->ShaderId);

		if (Program->Compile())
		{
			Program->Use();

			Programs.try_emplace(Count, Program);
			Count++;
		}
		else
		{
			delete Program;
			Program = nullptr;
		}

		glDeleteShader(VertexShader->ShaderId);
		glDeleteShader(PixelShader->ShaderId);
	}
	else
	{
		return;
	}

	delete VertexShader, PixelShader;
	VertexShader = nullptr;
	PixelShader = nullptr;
}

ShaderProgram* ShaderProgramMap::GetByKey(int Key)
{
	return Programs.find(Key)->second;
}
