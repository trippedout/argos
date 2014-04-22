#include "HBCategory.h"

HBCategory::HBCategory(std::string tit)
{
	title = tit; 

}

HBCategory::~HBCategory(void)
{
}

std::string HBCategory::getTitle()
{
	return title; 
}

std::vector< DataNodeRef >* HBCategory::getNodes()
{
	return &nodes; 
}

void HBCategory::addData(  DataNodeRef node )
{
	nodes.push_back(node); 
}

