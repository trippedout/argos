#pragma once
#include "DataView3dObject.h"
#include "HandbookRotationView.h"


typedef boost::shared_ptr<class HandbookView3dObject> HandbookView3dObjectRef;

class HandbookView3dObject :
	public DataView3dObject
{
public:
	HandbookView3dObject(DataNodeRef dn, std::string assetID, Assets* assets);
	~HandbookView3dObject(void);

	virtual void update(float dt); 
	virtual void onAnimateIn(); 
	virtual void onAnimateOut(); 

	virtual void show(); 
	virtual void hide(); 
	
	bool canRemove(){ return object->canRemove(); }; 
};

