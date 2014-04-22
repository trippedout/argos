#pragma once

#include "DataNode.h"

typedef boost::shared_ptr<class HBCategory> HBCategoryRef;

class HBCategory
{	
protected: 
	std::string title; 
	std::vector< DataNodeRef > nodes; 

public:
	HBCategory(std::string title);
	~HBCategory(void);

	std::string getTitle(); 
	std::vector< DataNodeRef >* getNodes(); 
	void addData(  DataNodeRef node );
};

