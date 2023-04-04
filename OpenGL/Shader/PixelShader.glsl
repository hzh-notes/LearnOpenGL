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
	int diffuse;
	sampler2D diffuseSampler;
	int specular;
	sampler2D specularSampler;
	int emission;
	sampler2D emissionSampler;
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

vec3 CalcDirLight(DirectionLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
{
	vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * diffuseColor;
    vec3 diffuse  = light.diffuse  * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
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
    vec3 ambient  = light.ambient  * diffuseColor;
    vec3 diffuse  = light.diffuse  * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;
    //ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
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
    vec3 ambient  = light.ambient  * diffuseColor;
    vec3 diffuse  = light.diffuse  * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;
	diffuse *= 1.0 * intensity;
	specular *= 1.0 * intensity;
	
	return (ambient + diffuse + specular);
}

void main()
{
	vec2 uv = 1.0f * vec2(TexCoord.x, 1.0 - TexCoord.y);
	
	vec4 finalColor;
	if(material.emission == 1)
	{
		vec3 emissionColor = texture(material.emissionSampler, uv).rgb;
		finalColor = vec4(1.0);//emissionColor;
	}
	else
	{
		vec3 norm = normalize(Normal);
		vec3 viewDir = normalize(viewPos - FragPos);
		
		vec4 baseColor = texture(material.diffuseSampler, uv);
		
		vec3 diffuse = mix(vec3(1.0), baseColor.rgb, material.diffuse);
		
		vec3 specular = mix(vec3(1.0), texture(material.specularSampler, uv).rgb, material.specular);
		
		finalColor.rgb += CalcDirLight(dirLight, norm, viewDir, diffuse, specular);
		finalColor.rgb += CalcSpotLight(spotLight, norm, FragPos, viewDir, diffuse, specular);
		finalColor.a = baseColor.a;
	}
	
	FragColor = finalColor;

}