#version 330 core
out vec4 color;
in vec3 normal;
in vec3 worldPos;
void main()
{
	//float depth_alpha = clamp(worldPos.z, -10.0, 0.0) / -10.0;
//	color = ((1 - depth_alpha) * normal) + ((vec3(1.0, 1.0, 1.0) * depth_alpha));
	color = vec4(1, 1, 0, 1);
	//color = normal;
	//color = vec3(1, 0, 0);
}
