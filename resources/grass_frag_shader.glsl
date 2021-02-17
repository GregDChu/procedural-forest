#version 330 core

uniform float fogDepth;
uniform sampler2D grassTexture;

out vec4 color;
in vec3 normal;
in vec3 worldPos;
in vec2 vertexTextPos;
void main()
{
	/*vec4 grassColor;
	float z_depth_alpha = clamp(worldPos.z, -10.0, 0.0) / -10.0;
	grassColor = texture(grassTexture, vertexTextPos);
	float intensity = length(grassColor.rgb) / 1.0;
	if(intensity < 0.01){
		discard;
	}*/
	float z_depth_alpha = clamp(worldPos.z, -fogDepth, 0.0) / -fogDepth;
	float intensity = length(texture(grassTexture, vertexTextPos).rgb) / 1.0;
	color = texture(grassTexture, vertexTextPos);
	if(color.a < 0.1)
	{
		discard;
	}
	else
	{
			color.rgb = (vec3(0.2, 0.2, 0.2) * (1.0 - z_depth_alpha)) + (vec3(1.0, 1.0, 1.0) * z_depth_alpha);
	}
	/*vec4 grassColor;
	
	//color.rgb = (1 - z_depth_alpha) * vec3(0, 0.8, 0.0);
	
	if(grassColor.a > 0)
	{
		color.rgb = vec3(1, 1, 1);
		color.rgb *= grassColor.rgb * (1.0 - z_depth_alpha);
		//color.rgb = (vec3(1, 1, 1) * z_depth_alpha) + (grassColor.rgb * (1 - z_depth_alpha));
	}
	//TODO Fix all of this grass stuff so it fades into the mist
	color.a = grassColor.a;
	//grassColor = texture(grassTexture, vertexTextPos).rgb;
	//color.rgb = (vec3(1, 1, 1) * z_depth_alpha) + (grassColor * (1 - z_depth_alpha));
	//color.rgb += z_depth_alpha;
	//color.rgb += 0.3;
	//color.rgb = vec3(1, 0, 0);
	//color.a = 1.0;
	//color = vec4(normal, 1.0);*/
}
