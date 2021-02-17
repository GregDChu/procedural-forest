#version 330 core

uniform float fogDepth;

out vec4 color;
in vec3 normal;
in vec3 worldPos;
void main()
{
	color.rgb = vec3(0, 1, 1);
	color.a = 0.8;
}
