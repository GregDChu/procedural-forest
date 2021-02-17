#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float heightSeed;
//uniform float fogDepth;

out vec3 normal;
out vec3 worldPos;
out vec2 vertexTextPos;
void main()
{
	vec4 tempPos;
	//float alpha = ;
	tempPos = vec4(vertPos, 1.0);
	//tempPos.y += pow(fract(sin(dot(tempPos.xz, vec2(12.6898,78.233))) * heightSeed) * 0.1, 2.0);
	normal = (vec4(normalize(vertNor), 0)).xyz;
	tempPos = V * M * tempPos;
	worldPos = tempPos.xyz;
	gl_Position = P * tempPos;
	vertexTextPos = vec2(vertTex.x, vertTex.y);
}
