#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
//uniform float fogDepth;

out vec3 normal;
out vec3 worldPos;
void main()
{
	vec4 tempPos;
	float height;
	normal = (vec4(normalize(vertNor), 0)).xyz;
	tempPos = vec4(vertPos, 1.0);
	height = fract(sin(cos(dot(tempPos.xyz, vec3(2.53, 452.34, 34.335)))) * 4006.314363423) * 0.2;
	tempPos.x += normal.x * height;
	tempPos.y += normal.y * height;
	tempPos = V * M * tempPos;
	worldPos = tempPos.xyz;
	gl_Position = P * tempPos;
}
