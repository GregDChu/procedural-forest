#pragma once

#ifndef H_MAP
#define H_MAP

#include "Asset.h"
//#include "Surface.h"
#include "config.h"

class Map
{
public:
	vector<Asset> mapAssets;
	vector<Asset> vegitation;
	vector<Asset> structures;
	vector<Asset> items;
	//vector<Surface> plants;
	float range;
	//float yRange;
	//float zRange;
	unsigned int assetInstances;

	Map(float range)
	{
		this->range = range;
		assetInstances = 0;
	}

	void clearAssetType(AssetType type)
	{
		for (int i = 0; i < mapAssets.size(); i++)
		{
			Asset asset = mapAssets.at(i);
			if (asset.isType(type))
				mapAssets.erase(mapAssets.begin() + i);
		}
	}

	vector<vec2> emptyAssetLocations(vec3 position, float gridSpacing, AssetType type, vec3 waterPosition)
	{
		//Generate new trees
		vector<vec2> emptyLocations;
		vec3 gridPosition;
		gridPosition.x = (int)(position.x / gridSpacing);
		gridPosition.y = 0;
		gridPosition.z = (int)(position.z / gridSpacing);
		int viewRadiusAsGrid = (int)ceil(FOG_DEPTH / gridSpacing);

		//TODO Make this(mapAsset and other asset vectors) into a hash table eventually 
		//so that access time to check if there is an asset of a specific type at the grid
		//location already. For now, we will have an O(n) check to see what assets already
		//exist in a current location
		for (int x = gridPosition.x - viewRadiusAsGrid; x <= gridPosition.x + viewRadiusAsGrid; x++)
		{
			for (int z = gridPosition.z - viewRadiusAsGrid; z <= gridPosition.z + viewRadiusAsGrid; z++)
			{
				bool gridIsEmpty = true;
				if ((x == 0 && z == 0) || abs(distance(vec3(x * gridSpacing, 0, z * gridSpacing), vec3(waterPosition.x, 0, waterPosition.z))) < WATER_HOLE_RADIUS + 0.3)
				{
					continue;
				}

				//TODO This check here will eventually be constant using a hash table
				for (int i = 0; i < mapAssets.size(); i++)
				{
					vec3 assetPos = mapAssets.at(i).getPosition();
					if (abs(distance(vec2(assetPos.x, assetPos.z), vec2(x * gridSpacing, z * gridSpacing))) <= mapAssets.at(i).getSpacingRadius())
					{
						gridIsEmpty = false;
						break;
					}
					if (mapAssets.at(i).isType(type)) {
						int assetGridPosX = (assetPos.x / gridSpacing);
						int assetGridPosZ = (assetPos.z / gridSpacing);
						if ((assetGridPosX == x && assetGridPosZ == z))
						{
							gridIsEmpty = false;
							break;
						}
					}
				}
				if (gridIsEmpty)
				{
					emptyLocations.push_back(vec2(x * gridSpacing, z * gridSpacing));
				}
			}
		}

		return emptyLocations;
	}


	void drawMap(mat4 P, mat4 V, vec3 position)
	{
		for (int i = 0; i < mapAssets.size(); i++)
		{
			Asset asset = mapAssets.at(i);
			if (abs(distance(asset.getPosition(), position)) <= range) {
				mapAssets.at(i).drawAsset(P, V);
			}
		}
	}
	void addItem(Asset item)
	{
		item.setID(++assetInstances);
		mapAssets.push_back(item);
	}
	bool isPopulated()
	{
		return mapAssets.size() > 0;
	}

	bool noCollisions(Asset asset)
	{
		for (int i = 0; i < mapAssets.size(); i++)
		{
			Asset compareAsset = mapAssets.at(i);
			vec3 assetGroundPos = vec3(asset.getPosition().x, 0, asset.getPosition().z);
			vec3 compareAssetGroundPos = vec3(compareAsset.getPosition().x, 0, compareAsset.getPosition().z);
			if (distance(assetGroundPos, compareAssetGroundPos) <= (compareAsset.getSpacingRadius() + asset.getSpacingRadius()))
				return false;
		}
		return true;
	}

	void removeItem(unsigned int id)
	{
		for (int i = 0; i < mapAssets.size(); i++)
		{
			if (mapAssets.at(i).getID() == id)
				mapAssets.erase(mapAssets.begin() + i);
		}
	}

	unsigned int closestInteractiveAsset(vec3 currentPos)
	{
		unsigned int closestID = 0;
		float minDistance = MIN_PICKUP_DISTANCE + 1;

		for (int i = 0; i < mapAssets.size(); i++)
		{
			Asset currentAsset = mapAssets.at(i);
			float dist = distance(currentPos, currentAsset.getPosition());
			if ((closestID == 0 || \
				dist < minDistance) && currentAsset.isRemovable())
			{
				minDistance = dist;
				closestID = currentAsset.getID();
			}
		}
		if (minDistance > MIN_PICKUP_DISTANCE)
			return 0;
		return closestID;
	}
};

#endif