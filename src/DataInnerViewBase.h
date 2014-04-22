#pragma once

#include "InputView.h"

#include "ARGOSButtonView.h"
#include "DataViewTitle.h"

#include "DataViewContentBase.h"
#include "DataViewOverview.h"
#include "DataViewDataList.h"
#include "DataViewWaypoint.h"
#include "DataNode.h"

class DataInnerViewBase : public InputView
{
	typedef boost::shared_ptr<DataViewTitle> DVTitleDisplayObjectRef;
	typedef boost::shared_ptr<DataViewContentBase> DVContentDisplayObjectRef;

	bool		mFirstRun;

protected:
	
	ImageViewRef lightBeam; 

	std::vector<ButtonViewRef> buttons; 
	std::vector<DVContentDisplayObjectRef> views; 

	boost::shared_ptr<DataNode>		dataNode;

	DVContentDisplayObjectRef	data;
	DVContentDisplayObjectRef	waypoint;
	
	ButtonViewRef				overviewBtn;
	ButtonViewRef				dataBtn;
	ButtonViewRef				waypointBtn;

	DVTitleDisplayObjectRef	mTitle;
	
	DVContentDisplayObjectRef	mCurrentContent;

	std::map<ButtonViewRef, DVContentDisplayObjectRef> mContentMap;

	ci::Vec3f			mCenterVec, mRightVec, mLeftVec;

	void			hideOffscreenViews();
	int				getViewIndex(DVContentDisplayObjectRef view); 


public:
	DataInnerViewBase(boost::shared_ptr<DataNode> dn,  Assets* assets);
	
	virtual void	onAnimateIn(); 
	virtual void	onAnimateOut(); 

	virtual void	prerender(); 
	virtual void	reset(); 
	virtual void	setSelection(ci::Vec2i selection);
	virtual void	handleInput(INPUT_TYPE type);
#ifdef _WIN32
	virtual void	handleGamepad(XINPUT_GAMEPAD gamepad);
#endif

	virtual void show(bool snap);
	virtual void hide(bool snap); 

	void closeView(); 

	~DataInnerViewBase(void);
};

