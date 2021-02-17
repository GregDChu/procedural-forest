#pragma once

#ifndef H_PLAYER
#define H_PLAYER

#include "Map.h"

#define MOVE_WALK_SPEED 2.5
#define MOVE_RUN_SPEED 5

class Player
{
public:
	float speed;
	enum MoveType {WALKING, RUNNING, STOPPED};
	vec3 collisionPos;

	Player()
	{
		speed = MOVE_WALK_SPEED;
	}
	float getSpeed()
	{
		return speed;
	}
	void setMovementType(MoveType moveType)
	{
		if (moveType == WALKING)
			speed = MOVE_WALK_SPEED;
		if (moveType == RUNNING)
			speed = MOVE_RUN_SPEED;
		if (moveType == STOPPED)
			speed = 0;
	}
	bool detectCollision(Map map, vec3 position)
	{
		for (int i = 0; i < map.mapAssets.size(); i++)
		{
			Asset currentAsset = map.mapAssets.at(i);
			vec3 currentAssetGroundPos = vec3(currentAsset.getPosition().x, 0, \
				currentAsset.getPosition().z);
			vec3 playerGroundPos = vec3(position.x, 0, position.z);
			float dist = distance(playerGroundPos, currentAssetGroundPos);
			if (currentAsset.isSolid() && dist < currentAsset.getSpacingRadius())
				return true;
		}
		return false;
		/*float minRadius = -1;
		for (int i = 0; i < map.mapAssets.size(); i++)
		{
			Asset currentAsset = map.mapAssets.at(i);
			if (currentAsset.isSolid() && (minRadius < 0 || currentAsset.getSpacingRadius() < minRadius))
				minRadius = currentAsset.getSpacingRadius();
		}
		//minRadius;
		vec3 futurePos = vec3(position.x + (cos(theta) * minRadius), \
			0, \
			position.z + (sin(theta) * minRadius));
		int i = -1;
		float dist;
		do {
			i++;
			Asset currentAsset = map.mapAssets.at(i);
			vec3 groundPos = vec3(currentAsset.getPosition().x, \
				0, \
				currentAsset.getPosition().z);
			dist = distance(groundPos, futurePos);
		} while (i + 1 < map.mapAssets.size() && dist > (map.mapAssets.at(i).getSpacingRadius()));

		float epsilon = 0.01;
		if (i + 1 < map.mapAssets.size()) {
			collisionPos = vec3(position.x - (sin(theta) * map.mapAssets.at(i).getSpacingRadius()) - epsilon, \
				position.y, \
				position.z - (cos(theta) * map.mapAssets.at(i).getSpacingRadius()) - epsilon);
			return true;
		}
		return false;
		//return newPos;*/
	}
	
};

#endif