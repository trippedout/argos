#pragma once

#include "Overlay.h"
#include "InputView.h"
#include "ImageView.h"
#include "TextView.h"

#include "HandbookBackgroundView.h"
#include "HandbookDataView.h"
#include "HandbookNavigationView.h"
#include "ScrubVideoObject.h"

#include "DataNode.h"
#include "BugView.h"

class HandbookView : public InputView
{
protected:
	ScrubVideoRef					mHandbookToLocation; 
	TextViewRef						mTitle;

	ImageViewRef					mAddBack;

	BugView*						bug;
	float							inputDelay; 
	bool							inData;
	HandbookNavigationViewRef		mNavigationView;
	HandbookDataViewRef				mDataView;
	HandbookBackgroundViewRef		mBackgroundView;

	void onDataSelected(DataNodeRef dataNode);
	void onLocationSelected(LocationNodeRef locationNode);
	void exitData(); 

public:
	HandbookView(BugView* bug, Assets* assets);
	~HandbookView(void);

	virtual void prerender(); 
	virtual void render();
	virtual void update(float dt);
	virtual void handleInput(INPUT_TYPE type);
#ifdef _WIN32
	virtual void handleGamepad( XINPUT_GAMEPAD gamepad );
#endif
	
	virtual void onAnimateIn(Settings::ViewID from); 
	virtual void onAnimateOut(Settings::ViewID to); 

	virtual void onAnimateIn(); 
	virtual void onAnimateOut(); 
	virtual void onAnimateOutComplete();
};