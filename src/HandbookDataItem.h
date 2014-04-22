#pragma once

#include "InputView.h"

#include "ARGOSButtonView.h"
#include "DataViewTitle.h"

#include "DataViewContentBase.h"

#include "HandbookViewOverview.h"
#include "HandbookViewDataList.h"
#include "HandbookViewWaypoint.h"
#include "DataNode.h"
#include "HandbookBackgroundView.h"

typedef boost::shared_ptr<class HandbookDataItem> HandbookDataItemRef;

class HandbookDataItem : public InputView
{
	typedef boost::shared_ptr<HandbookViewOverview> HandbookViewOverviewRef;
	typedef boost::shared_ptr<DataViewTitle> DVTitleDisplayObjectRef;
	typedef boost::shared_ptr<DataViewContentBase> DVContentDisplayObjectRef;

	
protected: 
	HandbookBackgroundViewRef background; 
	DataNodeRef dataNode; 

	std::vector<ButtonViewRef> buttons; 
	std::vector<DVContentDisplayObjectRef> views; 
	
	DVContentDisplayObjectRef	data;
	DVContentDisplayObjectRef	waypoint;
	
	ButtonViewRef				overviewBtn;
	ButtonViewRef				dataBtn;
	ButtonViewRef				waypointBtn;

	HandbookView3dObjectRef		object3D;

	DVTitleDisplayObjectRef	mTitle;
	DVContentDisplayObjectRef mCurrentContent;
	std::map<ButtonViewRef, DVContentDisplayObjectRef> mContentMap;

	void hideOffscreenViews();
	int	getViewIndex(DVContentDisplayObjectRef view); 

public:
	HandbookDataItem(HandbookBackgroundViewRef background, DataNodeRef node, Assets* assets);
	~HandbookDataItem(void);
	
	virtual void prerender(); 
	
	virtual void onAnimateIn(); 
	virtual void onAnimateOut(); 
	
	virtual void setSelection( ci::Vec2i selection );
	virtual void handleInput( INPUT_TYPE type );
#ifdef _WIN32
	virtual void handleGamepad( XINPUT_GAMEPAD gamepad );
#endif

	HandbookView3dObjectRef getObject3D(){return object3D;};
};