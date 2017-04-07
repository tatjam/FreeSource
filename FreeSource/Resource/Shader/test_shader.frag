#version 330 core
struct Material {
    sampler2D diffuse;
	vec3 diffuseValue;
	int diffuseEnabled;

    sampler2D specular;
	vec3 specularValue;
	int specularEnabled;

	sampler2D emissive;
	vec3 emissiveValue;
	int emissiveEnabled;
	float emissiveForce;

	sampler2D normalMap;
	int normalEnabled;

    float shininess;

	sampler2D reflectiveMap;
	int reflectiveMapEnabled;

	vec3 reflective;

	vec2 diffuseOffset;
	vec2 specularOffset;
	vec2 emissiveOffset;

};  

uniform sampler2D shadowMap;


uniform int skyboxEnabled;
uniform samplerCube skybox;

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;
uniform int dirLightActive;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define MAX_POINT_LIGHTS 4  
uniform int pointLightCount;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform float edgeSmooth;

uniform vec3 cameraPos;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
in vec4 FragPosLightSpace;

in mat3 TBN;

out vec4 color;
  
uniform vec3 viewPos;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow);
float CalculateShadow(vec4 fragPosLightSpace, float bias);

void main()
{

	vec3 I = normalize(FragPos - cameraPos);
    vec3 R = reflect(I, normalize(Normal));


    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = vec3(0, 0, 0);

	vec3 ambient = vec3(0, 0, 0);


	if(material.normalEnabled == 1)
	{
		vec3 normalTex = texture(material.normalMap, TexCoords).rgb;
		normalTex = normalize(normalTex * 2.0 - 1.0);
		normalTex = normalize(TBN * normalTex);


		norm = normalTex;
	}


	float shadow = 1.0f;

	if (dirLightActive != 0)
	{
		
		shadow = CalculateShadow(FragPosLightSpace, 0.001f);

		result += CalcDirLight(dirLight, norm, viewDir, shadow);

		if(material.diffuseEnabled == 1)
		{
			ambient = dirLight.ambient * vec3(texture(material.diffuse, TexCoords + material.diffuseOffset));
		}
		else
		{
			ambient = dirLight.ambient * material.diffuseValue;
		}
	}

	for (int i = 0; i < pointLightCount; i++)
	{
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, shadow);
	}



	result *= shadow;


	/* Reflection of skybox */
	if(skyboxEnabled == 1)
	{
		if(material.reflectiveMapEnabled == 1)
		{
		
		}
		else
		{
			result += vec3(texture(skybox, R)) * material.reflective;
		}
	}
	
	result += ambient;

	if(material.emissiveForce > 0 && material.emissiveEnabled == 1)
	{
		result += vec3(texture(material.emissive, TexCoords + material.emissiveOffset) * material.emissiveForce);
	}

	if(material.emissiveEnabled == 0)
	{
		result += vec3(material.emissiveValue) * vec3(material.emissiveForce);
	}


	//result = E;

    color = vec4(result, 1.0f);
} 


float CalculateShadow(vec4 fragPosLightSpace, float bias)
{

	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	projCoords = projCoords * 0.5 + 0.5;

	if(projCoords.z > 1.0)
        return 0.0f;
		
	float closestDepth = texture(shadowMap, projCoords.xy).r;   
	float currentDepth = projCoords.z;


	// Not smoothed, comment out for slightly better perfomance
	/*
	if(currentDepth - bias > closestDepth)
	{
		return 0.0f;
	}
	else
	{
		return 1.0f;
	}
	*/
	
	// Shadow smoothing

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
	return 1.0f - shadow;
	
	
}

// Many thanks to learnopengl.com for all these lighting functions

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow)
{
	/*if(material.normalEnabled == 1)
		normal = nnormal;*/

    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//spec *= diff;
    // Combine results
    vec3 diffuse;

	if(material.diffuseEnabled == 1)
	{
		diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords + material.diffuseOffset));
	}
	else
	{
		diffuse = light.diffuse * diff * material.diffuseValue;
	}

	vec3 specular;
	if(material.specularEnabled == 1)
	{
		specular = light.specular * spec * vec3(texture(material.specular, TexCoords + material.diffuseOffset));
	}
	else
	{
		specular = light.specular * spec * material.specularValue;
	}

	specular *= shadow;

    return ( diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading (maybe change lightDir to -lightDir?)
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));
	    vec3 diffuse;

	if(material.diffuseEnabled == 1)
	{
		diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	}
	else
	{
		diffuse = light.diffuse * diff * material.diffuseValue;
	}

	vec3 specular;
	if(material.specularEnabled == 1)
	{
		specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	}
	else
	{
		specular = light.specular * spec * material.specularValue;
	}

	specular *= shadow;

	diffuse *= attenuation;
	specular *= attenuation;
	return (diffuse + specular);
}