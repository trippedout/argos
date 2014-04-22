#pragma once

#include "DataNode.h"
#include "InputView.h"
#include "HandbookDataItem.h"
#include "HandbookBackgroundView.h"
#include <vector>

typedef boost::shared_ptr<class HandbookDataView> HandbookDataViewRef;

class HandbookDataView : public InputView
{
	
protected: 
	HandbookDataItemRef mRemoveItem; 
	HandbookDataItemRef mCurrentItem; 
	std::map< DataNodeRef, HandbookDataItemRef> mDataItems; 
	HandbookBackgroundViewRef mBackground;

	ci::Anim<float>	transition;
	void removeItem(); 

public:
	HandbookDataView( HandbookBackgroundViewRef background, Assets* assets);
	~HandbookDataView(void);
	
	virtual void prerender(); 
	
	virtual void onAnimateIn(); 
	virtual void onAnimateOut(); 
	
	virtual void handleInput( INPUT_TYPE type );
#ifdef _WIN32
	virtual void handleGamepad( XINPUT_GAMEPAD gamepad );
#endif

	virtual void setDataNode(DataNodeRef node);
};