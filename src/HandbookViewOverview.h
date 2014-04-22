#pragma once
#include "DataViewOverview.h"

class HandbookViewOverview :
	public DataViewOverview
{

public:
	HandbookViewOverview(DataNodeRef dn, Assets* assets);
	~HandbookViewOverview(void);

	virtual void onViewOpen(); 
	virtual void onViewClose(); 

	virtual void show(bool snap); 
};

