#pragma once

#include "Map.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;

static class MapPopulator
{
public:
	vec3 getPlacementRange(Map map, Asset asset, vec3 origin);
	vec3 genPlacement(vec3 origin, vec3 range);
	static void addVeg(Map map, Asset veg);
	static void addStructure(Map map, Asset structure);
};
