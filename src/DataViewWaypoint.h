#pragma once

#include "DataViewContentBase.h"
#include "DataImage.h"
#include "DataViewListIcon.h"
#include "DataNodeWaypointItem.h"
#include "TextViewMultiLine.h"
#include "TextView.h"

class DataViewWaypoint : public DataViewContentBase
{
typedef DataViewListIcon::DataViewListIconRef IconRef;

public:
	struct WaypointInfo 
	{
		std::string title; 
		std::string subtitle; 
		ci::gl::Texture* image; 

		WaypointInfo(std::string title, std::string subtitle, ci::gl::Texture* image) 
			: title(title), subtitle(subtitle), image(image){}
	};

	DataViewWaypoint(boost::shared_ptr<DataNode> dn, Assets* assets);
	~DataViewWaypoint(void);

	virtual void moveSelection(ci::Vec2i dir);
	virtual void handleInput(INPUT_TYPE type);
	
	virtual void show(bool snap); 
	virtual void hide(bool snap); 
	
	virtual void onViewOpen(); 
	virtual void onViewClose();

protected:

	float holderSnapY;
	ImageViewRef mGuide;
	ImageViewRef arrow; 
	ImageViewRef buy; 

	boost::shared_ptr<TextViewMultiline> title;
	boost::shared_ptr<TextViewMultiline> body;

	std::map<ButtonViewRef, WaypointInfo*> infoList; 
	std::vector<ButtonViewRef> icons;
	boost::shared_ptr<DataImage> image; 
	DisplayObjectContainerRef holder; 

};

