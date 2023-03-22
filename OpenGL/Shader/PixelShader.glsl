#version 430 core
#extension GL_ARB_shading_language_include : require

struct DirectionLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight 
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float innerCutOff;
	float outerCutOff;
	
	float constant;
    float linear;
    float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	bool bEmission;
	int shininess;
};

uniform vec3 viewPos;
uniform DirectionLight dirLight;
uniform SpotLight spotLight;
uniform Material material;

in vec4 VertexColor;
in vec3 Normal;
in vec2 TexCoord;

in vec3 FragPos;

out vec4 FragColor;

vec3 CalcDirLight(DirectionLight light, vec3 normal, vec3 viewDir, vec2 uv)
{
	vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, uv));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, uv));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, uv));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 uv)
{
	vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, uv));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, uv));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, uv));
    //ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 uv)
{
	vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity= clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	
	// 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, uv));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, uv));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, uv));
	diffuse *= 1.0 * intensity;
	specular *= 1.0 * intensity;
	
	return (ambient + diffuse + specular);
}

void main()
{
	vec2 uv = 2.0f * vec2(TexCoord.x, 1.0 - TexCoord.y);
	
	vec3 finalColor;
	if(material.bEmission)
	{
		vec3 emissionColor = texture(material.emission, uv).rgb;
		finalColor = vec3(100,1,1);//emissionColor;
	}
	else
	{
		vec3 norm = normalize(Normal);
		vec3 viewDir = normalize(viewPos - FragPos);
		finalColor += CalcDirLight(dirLight, norm, viewDir, uv);
		finalColor += CalcSpotLight(spotLight, norm, FragPos, viewDir, uv);
	}
	
	FragColor = vec4(finalColor, 1.0);

}