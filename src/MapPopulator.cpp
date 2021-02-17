#include "MapPopulator.h"
#include "iostream"

vec3 MapPopulator::genPlacement(vec3 origin, vec3 range)
{
	vec3 assetPosition;
	float x, y, z;
	x = range.x * ((float)rand() / RAND_MAX);
	y = range.y * ((float)rand() / RAND_MAX);
	z = range.z * ((float)rand() / RAND_MAX);
	assetPosition = vec3(x, y, z) + origin;
	return assetPosition;
}

/* origin - location on the map from which to deviate from*/
vec3 MapPopulator::getPlacementRange(Map map, Asset asset, vec3 origin)
{
/*#define UP_LEFT 0
#define LOW_LEFT 1
#define UP_RIGHT 2
#define PI 3.1415926535897
	float minDistances[3];
	Asset *closestSimilarAssets[3] = {0};
	Asset *currentAsset;
	float theta;
	float initDistance;
	vec3 *boundsVertices[3];

	initDistance = pow((pow(map.xRange, 2) + pow(map.yRange, 2) + (map.zRange, 2)), 0.5); // Pathag thrm
	minDistances[0] = initDistance;
	minDistances[1] = initDistance;
	minDistances[2] = initDistance;
	for (int i = 0; i < map.mapAssets.size(); i++)
	{
		 currentAsset = &(map.mapAssets.at(i));
		vec3 currentAssetPosition = currentAsset->getPosition();
		if (currentAsset->getType() == asset.getType())
		{
			float dist = distance(origin, currentAsset->getPosition());
			if (currentAssetPosition.x <= origin.x && currentAssetPosition.z <= origin.z)
			{
				
				if (dist <= minDistances[UP_LEFT])
				{
					minDistances[UP_LEFT] = dist;
					closestSimilarAssets[UP_LEFT] = currentAsset;
				}
			}
			else if (currentAssetPosition.x < origin.x && currentAssetPosition.z > origin.z)
			{
				if (dist <= minDistances[LOW_LEFT])
				{
					minDistances[LOW_LEFT] = dist;
					closestSimilarAssets[LOW_LEFT] = currentAsset;
				}
			}
			else if (currentAssetPosition.x >= origin.x && currentAssetPosition.z <= origin.z)
			{
				if (dist <= minDistances[UP_RIGHT])
				{
					minDistances[UP_RIGHT] = dist;
					closestSimilarAssets[UP_RIGHT] = currentAsset;
				}
			}
		}
	}*/

	/*std::cout << minDistances[UP_LEFT] << " "<<
		minDistances[UP_RIGHT] << " " <<
		minDistances[LOW_LEFT] << endl;
	std::cout << closestSimilarAssets[UP_LEFT] << " " <<
		closestSimilarAssets[UP_RIGHT] << " " <<
		closestSimilarAssets[LOW_LEFT] << endl;
	return vec3(0, 0, 0);*/
	/*float distances[3];
	distances[UP_LEFT] = distance(origin, closestSimilarAssets[UP_LEFT]->getPosition());
	distances[LOW_LEFT] = distance(origin, closestSimilarAssets[UP_LEFT]->getPosition());
	distances[UP_RIGHT] = distance(origin, closestSimilarAssets[UP_LEFT]->getPosition());

	boundsVertices[UP_LEFT] = &(vec3(closestSimilarAssets[UP_LEFT]->getPosition()));
	boundsVertices[LOW_LEFT] = &(vec3(boundsVertices[UP_LEFT]))*/
	return vec3(0, 0, 0);
}