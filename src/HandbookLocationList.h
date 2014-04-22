#pragma once
#include "HandbookList.h"
#include "Assets.h"

class HandbookLocationList :
	public HandbookList
{
protected: 
	virtual void buildListItems();
	virtual void updateData();

	typedef std::vector<LocationNodeRef>* LocationList;
	LocationList mLocations; 

public:
	HandbookLocationList(Assets* assets);
	~HandbookLocationList(void);

	virtual void handleInput(INPUT_TYPE type);
};

