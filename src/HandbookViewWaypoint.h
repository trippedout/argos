#pragma once
#include "DataViewWaypoint.h"

class HandbookViewWaypoint :
	public DataViewWaypoint
{
public:
	HandbookViewWaypoint(DataNodeRef dn, Assets* assets);
	~HandbookViewWaypoint(void);

	virtual void onViewClose(); 
	virtual void show(bool snap); 
	//virtual void hide(bool snap); 
};

