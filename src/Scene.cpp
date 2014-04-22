#include "Scene.h"


Scene::Scene(float s)
{
	start = s; 
	currentData = new std::vector<DataNodeRef>();
	locked = true;
}

std::vector< DataNodeRef  >* Scene::getDataNodes()
{
	return &data; 
}

std::vector< LocationNodeRef >* Scene::getLocationNodes()
{
	return &locations;
}

std::vector< DataNodeRef >* Scene::getCurrentDataNodes(float time)
{
	delete currentData;
	currentData = new std::vector<DataNodeRef>();
	
	int size = timeNodes.size();

	for( int i=0; i<size; i++){
		if(timeNodes[i].time <= time){
			currentData->push_back(timeNodes[i].node);
		}
	}

	return currentData;
}

LocationNodeRef Scene::getCurrentLocation(float time)
{
	int size = timeLocNodes.size();

	//theres only one you dummy. 
	if(size == 1) {
		timeLocNodes[0].node->setCurrentLocationID(timeLocNodes[0].locationID); 
		return timeLocNodes[0].node;
	}

	for( int i=0; i<size; i++){
		if(timeLocNodes[i].time <= time){
			timeLocNodes[i].node->setCurrentLocationID(timeLocNodes[0].locationID); 
			return timeLocNodes[i].node;
		}
	}

	//nothin there bro.
	return NULL;
}

void  Scene::addBeacon(BeaconData* val)
{
	beacons.push_back( BeaconDataRef( val ) ); 
}

std::vector< BeaconDataRef >* Scene::getBeacons()
{
	return &beacons;
}

void Scene::addData( DataNodeRef val, float appears )
{
	data.push_back(val);
	timeNodes.push_back( TimeNode(appears, val) ); 
}

void Scene::addLocation( LocationNodeRef val, float appears, std::string locationID )
{
	locations.push_back(val);
	timeLocNodes.push_back( TimeLocNode(appears, val, locationID) ); 
}

float Scene::getStart()
{
	return start;
}

bool Scene::isCurrent(float val)
{
	if( val > start ) return true; 

	else return false; 
}

bool Scene::isLocked()
{
	return locked; 
}

void Scene::setLocked(bool val)
{
	locked = val;
}

bool Scene::isOni()
{
	int size = data.size();
	
	for(int i=0; i<size; i++){
		if(data[i]->isOni()){
			return true; 
		}
	}

	return false; 
}

Scene::~Scene(void)
{
}
