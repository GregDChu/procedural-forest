#include "Bag.h"

Bag::Bag()
{
	markerUsed = false;
}

void Bag::addItem(Map map, unsigned int id)
{
	
	int i = 0;
	while(i < map.mapAssets.size() && \
		map.mapAssets.at(i).getID() != id)
	{
		i++;
	}
	if (i < map.mapAssets.size())
		items.push_back(map.mapAssets.at(i));
}
