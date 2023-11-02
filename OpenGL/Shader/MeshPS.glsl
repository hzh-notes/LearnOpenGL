#version 330 core
#extension GL_ARB_shading_language_include : require

struct DirectionLight
{
	vec3 direction;
	
	vec3 color;
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

uniform mat4 lightSpaceMat;
uniform sampler2D depthTexture;

in vec4 VertexColor;
in vec3 Normal;
in vec2 TexCoord;

in vec3 FragPos;

out vec4 FragColor;

vec3 CalcDirLight(DirectionLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor, float shadow)
{
	vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.f);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * diffuseColor;
    vec3 diffuse  = light.diffuse  * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;
    return (ambient + (1.f - shadow) * (diffuse + specular)) * light.color;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
{
	vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.f);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.f / (light.constant + light.linear * distance + 
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
    float diff = max(dot(normal, lightDir), 0.f);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity= clamp((theta - light.outerCutOff) / epsilon, 0.f, 1.f);
	
	// 合并结果
    vec3 ambient  = light.ambient  * diffuseColor;
    vec3 diffuse  = light.diffuse  * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;
	diffuse *= 1.f * intensity;
	specular *= 1.f * intensity;
	
	return (ambient + diffuse + specular);
}

void main()
{
	vec2 uv = 1.f * vec2(TexCoord.x, 1.f - TexCoord.y);
	
	vec4 finalColor;
	if(material.emission == 1)
	{
		vec4 emissionColor = texture(material.emissionSampler, uv);
		finalColor = emissionColor;
	}
	else
	{
		vec3 norm = normalize(Normal);
		vec3 viewDir = normalize(viewPos - FragPos);
		
		vec4 baseColor = texture(material.diffuseSampler, uv);
		
		vec3 diffuse = mix(vec3(1.0f), baseColor.rgb, material.diffuse);
		
		vec3 specular = mix(vec3(0.f), texture(material.specularSampler, uv).rgb, material.specular);
		
		//计算阴影
		vec4 lightSpaceFrag = lightSpaceMat * vec4(FragPos, 1.f);
		lightSpaceFrag.xyz /= lightSpaceFrag.w;
		lightSpaceFrag.xyz = lightSpaceFrag.xyz * 0.5f + 0.5f;
		float bias = max(0.00005f * (1.f - dot(Normal, dirLight.direction)), 0.000005f);
		float shadow = 0.f;
		vec2 texelSize = 1.f / textureSize(depthTexture, 0);
		for(int x = -1; x <= 1; ++x)
		{
			for(int y = -1; y <= 1; ++y)
			{
				float pcfDepth = texture(depthTexture, lightSpaceFrag.xy + vec2(x, y) * texelSize).r;
				float curShadow = lightSpaceFrag.z - bias > pcfDepth ? 1.f : 0.f;
				if(lightSpaceFrag.z > 1.f)
					curShadow = 0.f;
				shadow +=  curShadow;
			}
		}
		shadow /= 9.f;
		
		finalColor.rgb += CalcDirLight(dirLight, norm, viewDir, diffuse, specular, shadow);
		//finalColor.rgb += CalcSpotLight(spotLight, norm, FragPos, viewDir, diffuse, specular);
		finalColor.a = baseColor.a;
	}
	
	vec3 mapped = finalColor.rgb / (finalColor.rgb + 1.);
	mapped = pow(mapped, vec3(1. / 2.2));
	
	FragColor = vec4(mapped, finalColor.a);

}