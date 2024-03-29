#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 VertexColor;
out vec3 Normal;
out vec2 TexCoord;

out vec3 FragPos;
//out mat4 WorldToClip;

void main()
{

	FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(FragPos, 1.0);
	
    VertexColor = aColor;
	Normal = aNormal;//mat3(transpose(inverse(model)))
    TexCoord = aTexCoord;
	
}