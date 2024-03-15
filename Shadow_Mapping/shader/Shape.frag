#version 410 core

in vec3 objectColor;
in vec3 worldPos;
in vec2 texCoord;
in mat3 tbn;
in vec4 posLightSpace;

uniform sampler2D shapetex;
uniform sampler2D shapetexSpec;
uniform sampler2D shapetexNormal;
uniform sampler2D shadows;

uniform vec3 camPos;

out vec3 color;

uniform struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} light;

float compute_shadow(vec3 normal, vec3 lightDir)
{
	// TODO: complete the shadow evaluation
    vec3 projCoord = posLightSpace.xyz / posLightSpace.w;
    projCoord = projCoord * 0.5 + 0.5;
    float closestDepth = texture(shadows, projCoord.xy).r;
    float currentDepth = projCoord.z;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    return currentDepth - bias > closestDepth  ? 1.0 : 0.0;
}

void main()
{
	//vec3 colorMap = objectColor;
	vec3 colorMap = texture(shapetex, texCoord.xy).rgb;
	float specularMap = texture(shapetexSpec, texCoord.xy).r;

	vec3 normal = texture(shapetexNormal, texCoord.xy).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(tbn * normal);

	float ambientFactor = 0.2f;
	vec3 ambientColor = (light.ambient.xyz * ambientFactor);

	vec3 lightDir = vec3(normalize(light.position - worldPos));
	float diffDot = max(dot(normal, lightDir), 0.0);
	vec3 diffuseColor = diffDot * light.diffuse;

	vec3 viewDir = vec3(normalize(camPos - worldPos));
	vec3 reflectDir = reflect(-lightDir, normal);
	float specDot = max(dot(viewDir, reflectDir), 0.0);
	float spec = pow(specDot, 32);
	float specStrength = 1.0;
	vec3 specularColor = specStrength * spec * light.specular;

	// shadow mapping
	float shadow = 1.0 - compute_shadow(normal, lightDir);

	color = (ambientColor + shadow * diffuseColor) * colorMap.rgb + (shadow * specularColor * specularMap);
}

