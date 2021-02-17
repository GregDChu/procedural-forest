#version 330 core

uniform float fogDepth;
uniform sampler2D glowTexture;

out vec4 color;
in vec3 normal;
in vec3 worldPos;
in vec2 texPos;
void main()
{
	float z_depth_alpha = clamp(worldPos.z, -fogDepth, 0.0) / -fogDepth;
	float intensity = length(texture(glowTexture, texPos).rgb) / 1.0;
	//color = texture(glowTexture, texPos);
	//color.a = length(color.rgb) / 1.2;
	if(intensity < 0.01)
		discard;
	color.rgb = vec3(1, 1, 1);
	color.a = intensity;
	//color.rgb = vec3(1, 1, 1);
	//color.a = intensity;
	//color.rgb = (1 - z_depth_alpha) * vec3(0.1, 0.1, 0.1);
	//color.rgb += (1.0 * z_depth_alpha);
	//color.a = 1.0;
	//color = vec4(normal, 1.0);
}
