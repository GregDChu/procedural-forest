#version 330 core

uniform float fogDepth;
uniform float colorType;

out vec4 color;
in vec3 normal;
in vec3 worldPos;
void main()
{
	vec3 bodyColor;
	float z_depth_alpha = clamp(worldPos.z, -fogDepth, 0.0) / -fogDepth;
	//color.rgb = (1 - z_depth_alpha) * vec3(0.5, 0.5, 0.5);
	//color.rgb += (1.0 * z_depth_alpha);
	//color.a = 1.0;
	//color = vec4(normal, 1.0);
	bodyColor = (normal * 0.5) + (0.5 * vec3(mod(colorType + 0.6, 1.0), mod(colorType + 0.8, 1.0), mod(colorType + 0.3, 1.0)));
	color.rgb = (vec3(1, 1, 1) * z_depth_alpha) + (bodyColor * (1 - z_depth_alpha));
	color.a = 1.0;
}
