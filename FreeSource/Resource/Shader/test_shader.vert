#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bittangent;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightMatrix;

void main()
{



    gl_Position = projection * view *  model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;  

	// Could probably need texture coords inversion
	TexCoords = texCoords;

	FragPosLightSpace = lightMatrix * vec4(FragPos, 1.0);

	// Normal mapping and tangents
	vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(bittangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(normal,    0.0)));

	TBN = mat3(T, B, N);
} 