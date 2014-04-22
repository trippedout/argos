#pragma once

#include <boost/shared_ptr.hpp>

#include "BeaconData.h"
#include "LocationNode.h"
#include "DataNode.h"
#include "HBCategory.h"

#include "Scene.h"
#include "cinder/Json.h"
#include "cinder/app/AppNative.h"

#include <vector>
#include <iterator>
#include <string>

//using namespace ci;
//using namespace ci::app;
//using namespace std;

class DataController
{
	
protected: 
	int currentScene; 

	LocationNodeRef handbookLocation; 
	SceneRef pauseScene; 

	std::vector< DataNodeRef > data;
	std::vector< SceneRef > scenes;
	std::vector< LocationNodeRef > locations;

	std::vector< HBCategoryRef > hbItems;
	std::vector< HBCategoryRef > hbCharacters;
	std::vector< HBCategoryRef > hbOniFiles;

	bool mEngageComplete;
	bool mEpisodeComplete;
	bool mGamePulsed; 

public:
	typedef std::vector< LocationNodeRef >::iterator LocationIterator;
	typedef std::vector< DataNodeRef >::iterator DataIterator;
	typedef std::vector< SceneRef >::iterator SceneIterator;

	struct UnlockedInfo 
	{
		int items; 
		int characters; 
		int locations; 
		int onis; 

		int itemsTotal; 
		int charactersTotal; 
		int locationsTotal; 
		int onisTotal; 

		UnlockedInfo(int items, int characters, int locations, int onis, int itemsTotal, int charactersTotal, int locationsTotal, int onisTotal) 
			: items(items), characters(characters), locations(locations), onis(onis), itemsTotal(itemsTotal), charactersTotal(charactersTotal), locationsTotal(locationsTotal), onisTotal(onisTotal) {}
	};

	DataController(cinder::JsonTree tree);
	~DataController(void);

	std::vector< SceneRef >* getScenes(); 
	std::vector< DataNodeRef >* getDataNodes(); 
	std::vector< LocationNodeRef >* getLocationNodes(); 

	std::vector< HBCategoryRef >* getHBItems(); 
	std::vector< HBCategoryRef >* getHBCharacters(); 
	std::vector< HBCategoryRef >* getHBOniFiles(); 

	SceneRef  getCurrentScene(float time); 
	std::vector< DataNodeRef >* getCurrentSceneDataNodes(float time); 
	LocationNodeRef getCurrentSceneLocation(float time);

	UnlockedInfo getUnlocked(); 
	
	void setEpisodeComplete(bool complete = true) { mEpisodeComplete = complete; };
	bool getEpisodeComplete() { return mEpisodeComplete; };

	void setEngageComplete(bool complete);
	bool getEngageComplete() { return mEngageComplete; };

	void setGamePulsed(bool val = true) { mGamePulsed = val; };
	bool getGamePulsed() { return mGamePulsed; };

	void addGhettoData(); 

	void unlockAllContent(); 
	void lockAllContent(); 
	
	void setHandbookLocation(LocationNodeRef val){ handbookLocation = val; };
	void setPauseScene(SceneRef val){ pauseScene = val; };

	LocationNodeRef getHandbookLocation(){ return handbookLocation; }; 
	SceneRef getPauseScene(){ return pauseScene; }; 
};
