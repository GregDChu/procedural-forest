#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 offset;
uniform vec3 cameraPos;
out vec3 normal;
out vec3 relativePos;
out vec3 worldPos;
void main()
{
	vec4 tempPos;
	normal = (vec4(normalize(vertNor), 0)).xyz;
	tempPos = vec4(vertPos, 1.0);
	tempPos.x += offset.x;
	tempPos.z += offset.z;
	relativePos = tempPos.xyz;
	tempPos = V * M * tempPos;
	worldPos = tempPos.xyz;
	gl_Position = P * tempPos;
}
