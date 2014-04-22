#pragma once

#include "ButtonView.h"
#include "ImageView.h"
#include "TextView.h"
#include "DataNode.h"
#include "LocationNode.h"

class HandbookListSubItem : public ButtonView
{
protected: 
	ImageViewRef bg1; 
	ImageViewRef bg2; 
	ImageViewRef bg3; 
	
	ImageViewRef lock; 

	TextViewRef text; 
	TextViewRef textHighlight; 

	DataNodeRef mDataNode;
	LocationNodeRef mLocationNode;

	bool mLocked;
	std::string locationID;

	DisplayObjectContainerRef holder; 

	void init(); 

	int constellation; 

public:
	HandbookListSubItem(Assets* assets, std::string copy, DataNodeRef dataNode);
	HandbookListSubItem(Assets* assets, std::string copy, std::string locationID, LocationNodeRef locationNode);
	~HandbookListSubItem(void);

	std::string getLocationID(){return locationID;};
	virtual void onAnimateIn(float delay);
	virtual void onAnimateIn();
	virtual void onAnimateOut();
	virtual void selectView(bool select);

	DataNodeRef getDataNode() { return mDataNode; };
	LocationNodeRef getLocationNode() { return mLocationNode; };

	void setLocked(bool val); 
	bool getLocked(){ return mLocked; }; 

	int getConstellation(){return constellation;}; 

	static int subItemNum;
};

