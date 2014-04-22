#include "DataNodeWaypointItem.h"

using namespace std;

DataNodeWaypointItem::DataNodeWaypointItem(std::string ass, std::string tit, std::string typ, std::string bod)
{
	type = typ; 
	assetID = ass;
	title = tit; 
	body = bod; 
}

DataNodeWaypointItem::~DataNodeWaypointItem(void)
{

}
