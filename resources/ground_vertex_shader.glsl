#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
//layout(location = 2) in vec2 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 offset;
uniform vec3 cameraPos;
uniform vec3 nearestWater;
uniform float waterHoleRadius;
uniform float waterHoleDepth;
uniform float ratio;
uniform float density;

out vec3 normal;
out vec3 relativePos;
out vec3 worldPos;
out vec2 texturePos;

void main()
{
	vec4 tempPos;
	float vertDistToWaterPos;
	normal = (vec4(normalize(vertNor), 0)).xyz;
	
	tempPos = vec4(vertPos, 1.0);
	tempPos.x += offset.x;
	tempPos.z += offset.z;
	vertDistToWaterPos = distance(tempPos.xyz, nearestWater);
	if(vertDistToWaterPos < waterHoleRadius){
//		tempPos.y = -WATER_HOLE_DEPTH * ((1.0 - tanh(10 * (abs(distToWaterPos/WATER_HOLE_RADIUS) - 0.7))) / 2);
		tempPos.y = -waterHoleDepth * ((1.0 - tanh(10 * (abs(vertDistToWaterPos/waterHoleRadius) - 0.7))) / 2);
		//tempPos.y -= fract(sin(dot(tempPos.xz, vec2(12.6898,78.233))) * 43758.5453123) * 0.1;
	}
	//tempPos.y += fract(sin(dot(tempPos.xz, vec2(12.6898,78.233))) * 43758.5453123) * 0.4;
	relativePos = tempPos.xyz;
	//tempPos.x += offset.x;
	//tempPos.z += offset.z;
	tempPos = V * M * tempPos;
	//normal.x = fract(sin(dot(tempPos.xz, vec2(12.6898,78.233))) * 43758.5453123);
	//normal.y = fract(sin(dot(tempPos.xz, vec2(7.452,45.233))) * 43758.5453123);
	//normal.z = fract(sin(dot(tempPos.xz, vec2(61.68938,8.44))) * 43758.5453123);
	worldPos = tempPos.xyz;
	gl_Position = P * tempPos;
	texturePos = vec2(((mod(vertPos.x, ratio * 2) * density) / 17.5) * 1000, ((mod(vertPos.y, ratio * 2) * density) / 17.5) * 1000);
}
