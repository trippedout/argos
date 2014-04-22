#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>

//using namespace std; 

class DataNodeWaypointItem
{
protected: 

public:
	DataNodeWaypointItem(std::string ass, std::string tit, std::string type, std::string bod);
	~DataNodeWaypointItem(void);

	std::string assetID, title, type, body; 
};

