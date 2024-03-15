#version 410 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec2 vTexCoords;
layout(location = 4) in vec3 vTangent;

out vec3 fragColor;

uniform mat4 mvpMatrix;

uniform sampler2D linestexNormal;

void main()
{
	if((gl_VertexID & 1) == 1)
	{
		gl_Position = mvpMatrix * vec4(vPosition, 1);
	}
	else
	{
		mat3 tbn = mat3(vTangent, cross(vNormal, vTangent), vNormal);

		vec3 normal = texture(linestexNormal, vTexCoords.xy).rgb;
		normal = normalize(normal * 2.0 - 1.0);
		normal = normalize(tbn * normal);

		gl_Position = mvpMatrix * vec4(vPosition + normal, 1);
	}

	fragColor = vColor;
}

