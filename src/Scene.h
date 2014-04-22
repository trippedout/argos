#pragma once

#include "DataNode.h"
#include "BeaconData.h"
#include "LocationNode.h"

typedef boost::shared_ptr<class Scene> SceneRef;

class Scene
{

public:
	Scene(float s);
	~Scene(void);
	
	bool isOni(); 
	bool isLocked(); 
	void setLocked(bool val); 

	std::vector< BeaconDataRef >* getBeacons(); 
	std::vector< LocationNodeRef >* getLocationNodes(); 
	std::vector< DataNodeRef >* getDataNodes(); 
	std::vector< DataNodeRef >* getCurrentDataNodes(float time); 

	LocationNodeRef getCurrentLocation(float time);

	void addBeacon(BeaconData* val); 
	void addData( DataNodeRef val, float appears); 
	void addLocation( LocationNodeRef val, float appears, std::string locationID); 

	bool isCurrent(float time); 

	float getStart(); 

	struct TimeNode 
	{
		float time;
		DataNodeRef node; 
		TimeNode(float time, DataNodeRef node) : time(time), node(node) {}
	};

	struct TimeLocNode 
	{
		float time;
		LocationNodeRef node; 
		std::string locationID; 
		TimeLocNode(float time, LocationNodeRef node, std::string locationID) : time(time), node(node), locationID(locationID) {}
	};

protected: 

	std::vector< BeaconDataRef > beacons; 
	std::vector< TimeNode > timeNodes;
	std::vector< TimeLocNode > timeLocNodes;
	std::vector< DataNodeRef  > data;
	std::vector< LocationNodeRef  > locations;
	std::vector< DataNodeRef  >* currentData;
	float start;
	bool locked; 
};

