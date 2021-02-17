#pragma once

#ifndef H_BAG
#define BAG

#include "Asset.h"
#include "Map.h"

class Bag
{
public:
	vector<Asset> items;
	bool markerUsed;

	Bag();
	void addItem(Map map, unsigned int id);
};

#endif