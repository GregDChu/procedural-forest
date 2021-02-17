#version 330 core
out vec4 color;
in vec3 normal;
in vec3 relativePos;
in vec3 worldPos;
void main()
{
	float z_depth_alpha = clamp(worldPos.z, -10.0, 0.0) / -10.0;
	color = vec4(0.72, 0.05, 0.16, 0.4);
	color += z_depth_alpha * 0.4;
}
