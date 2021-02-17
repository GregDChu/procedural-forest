#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform mat4 camRotation;
//uniform float fogDepth;

out vec3 normal;
out vec3 worldPos;
void main()
{
	vec4 tempPos;
	normal = (vec4(normalize(vertNor), 0)).xyz;
	tempPos = vec4(vertPos, 1.0);
	tempPos = M * tempPos;
	tempPos.z -= 2.0;
	//tempPos = V * tempPos;
	worldPos = tempPos.xyz;
	gl_Position = P * tempPos;
}
