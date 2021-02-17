#version 330 core

uniform vec3 cameraPos;
uniform float fogDepth;
uniform float waterHoleDepth;
uniform sampler2D groundTexture;

out vec4 color;
in vec3 normal;
in vec3 relativePos;
in vec3 worldPos;
in vec2 texturePos;
void main()
{
	//The normal vector of the surface
	vec3 norm = normalize(normal);
	//The position of the light in world space
	vec3 light_source = vec3(0, 100, 0);
	//The vector created by the point being drawn and the light source
	vec3 light_vec = normalize(light_source - worldPos);
	vec3 camera_vec = normalize(worldPos - cameraPos);
	//vec3 half_vec = normalize(camera_vec + light_vec);
	//float specular = dot(norm, half_vec);
	float diffuse = dot(norm, light_vec);
	//color = vec3(0, 1, 1);
	float z_depth_alpha = clamp(worldPos.z, -fogDepth, 0.0) / -fogDepth;
	float y_depth_alpha = clamp(relativePos.y, -waterHoleDepth, 0.0) / -(waterHoleDepth + 0.2);
	//color.rgb = vec3(1, 0.2, 0.2) * diffuse;
	if(relativePos.y > -0.1)
		color.rgb = ((1 - z_depth_alpha) * vec3(0.6, 0.6, 0.6) * diffuse);
	else
		color.rgb = ((1 - z_depth_alpha) * vec3(1, 1, 1) * diffuse);
	color.rgb *= texture(groundTexture, texturePos).rgb;
	color.rgb += pow((1.0 * z_depth_alpha), 2);
	color.rgb *= (1 - y_depth_alpha);
	
	color.a = 1;
	//color = normal;
	//color = vec3(1, 0, 0);
}
