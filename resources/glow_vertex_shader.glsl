#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
//uniform float fogDepth;

out vec3 normal;
out vec3 worldPos;
out vec2 texPos;
void main()
{
	vec4 tempPos;
	normal = (vec4(normalize(vertNor), 0)).xyz;
	tempPos = V * M * vec4(vertPos, 1.0);
	worldPos = tempPos.xyz;
	gl_Position = P * tempPos;
	texPos = vec2(vertTex.x, vertTex.y);
}
