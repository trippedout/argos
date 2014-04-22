#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>

//using namespace std; 

class DataNodeDataItem
{
protected: 

public:
	enum DataItemType{MOVIE, OBJECT3D, DOCUMENT, AUDIO};

	DataNodeDataItem(DataItemType typ, std::string ass);
	DataNodeDataItem(DataItemType typ, std::string ass, std::string path );
	DataNodeDataItem(DataItemType typ, std::string ass, std::string path, std::string iconLabel);
	~DataNodeDataItem(void);

	std::string assetID; 
	std::string assetPath;
	std::string iconLabel;
	DataItemType type;
};

