#version 330 core

uniform float fogDepth;

out vec4 color;
in vec3 normal;
in vec3 worldPos;
void main()
{
	float z_depth_alpha = clamp(worldPos.z, -fogDepth, 0.0) / -fogDepth;
	color.rgb = (1 - z_depth_alpha) * vec3(0.1, 0.1, 0.1);
	color.rgb += (1.0 * z_depth_alpha);
	color.a = 1.0;
	//color = vec4(normal, 1.0);
}
