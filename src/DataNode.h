#pragma once

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "DataNodeDataItem.h"
#include "DataNodeWaypointItem.h"

//using namespace std; 

typedef boost::shared_ptr<class DataNode> DataNodeRef;

class DataNode
{
protected: 
	std::vector<DataNodeDataItem*> dataItems; 
	std::vector<DataNodeWaypointItem*> waypointItems; 
	
	bool locked, oni, oniLocked, oniPulsed, alwaysUnlocked;

public:
	enum DataType{ITEM, CHARACTER, ONI};
	
	bool isOniPulsed(){return oniPulsed;};
	void setOniPulsed(bool val){oniPulsed = val;};

	bool isOni(){return oni;};
	void setOni(bool val){oni = val;};

	bool isOniLocked(){return oniLocked;};
	void setOniLocked(bool val){oniLocked = val;}; 

	bool isLocked(){return locked;};
	void setLocked(bool val); 
	
	bool isAlwaysUnlocked() { return alwaysUnlocked; };
	void setAlwaysUnlocked(bool val) { alwaysUnlocked=val; if(alwaysUnlocked) locked = false; }; 

	std::string iconID, imageID, title, subtitle, body;
	
	DataType type; 

	void addDataItem(DataNodeDataItem* item); 
	void addWaypointItem(DataNodeWaypointItem* item); 

	std::vector<DataNodeDataItem*>* getDataItems(); 
	std::vector<DataNodeWaypointItem*>* getWaypointItems(); 

	DataNode( DataType typ, std::string icon, std::string image, std::string tit, std::string sub, std::string bod );
	DataNode( DataType typ, std::string icon, std::string image, std::string tit, std::string sub, std::string bod, bool hasOni );
	~DataNode(void);
};
