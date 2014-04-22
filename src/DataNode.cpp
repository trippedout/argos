#include "DataNode.h"

using namespace std;

DataNode::DataNode( DataType typ, std::string icon, std::string image, std::string tit, std::string sub, std::string bod  ):
	type(typ),
	iconID(icon),
	imageID(image),
	title(tit),
	subtitle(sub),
	body(bod),
	alwaysUnlocked(false),
	oni(false),
	locked(true),
	oniLocked(true),
	oniPulsed(false)
{
}


void DataNode::setLocked(bool val)
{
	locked = val;
	if(alwaysUnlocked) locked = false;
}


void DataNode::addDataItem(DataNodeDataItem* item)
{
	dataItems.push_back(item); 
}

void DataNode::addWaypointItem(DataNodeWaypointItem* item)
{
	waypointItems.push_back(item); 
}

std::vector<DataNodeDataItem*>* DataNode::getDataItems()
{
	return &dataItems;
}

std::vector<DataNodeWaypointItem*>* DataNode::getWaypointItems()
{
	return &waypointItems;
}

DataNode::~DataNode(void)
{
}
