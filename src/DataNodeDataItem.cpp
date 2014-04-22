#include "DataNodeDataItem.h"

using namespace std;

DataNodeDataItem::DataNodeDataItem(DataItemType typ, std::string ass)
{
	type = typ;
	assetID = ass; 
	assetPath = ""; 
	iconLabel = ""; 
}

DataNodeDataItem::DataNodeDataItem(DataItemType typ, std::string ass, std::string path)
{
	type = typ;
	assetID = ass; 
	assetPath = path; 
	iconLabel = ""; 
}

DataNodeDataItem::DataNodeDataItem(DataItemType typ, std::string ass, std::string path, std::string label)
{
	type = typ;
	assetID = ass; 
	assetPath = path; 
	iconLabel = label; 
}

DataNodeDataItem::~DataNodeDataItem(void)
{
}
