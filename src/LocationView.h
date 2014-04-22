#pragma once

#include "InputView.h"
#include "ARGOSButtonView.h"
#include "TextView.h"
#include "TextViewMultiline.h"
#include "VideoObject.h"
#include "ImageView.h"

#include "LocationBaseView.h"
#include "LocationOrbitalView.h"
#include "LocationAerialView.h"
#include "LocationLocalView.h"
#include "LocationTransitionView.h"

#include "DataController.h"
#include "LocationNode.h"
#include "BugElement.h"

class LocationView : public InputView
{
	typedef boost::shared_ptr<TextViewMultiline> TvMultiDisplayObjectRef;
	typedef boost::shared_ptr<LocationBaseView> LocationViewRef;
	typedef boost::shared_ptr<LocationTransitionView> LocationTransitionRef;
	typedef boost::shared_ptr<LocationNode> LocationNodeRef;
protected:
	VideoViewRef		mMainVideo;

	ButtonViewRef		mBtnOrbital;
	ButtonViewRef		mBtnAerial;
	ButtonViewRef		mBtnLocal;
	ButtonViewRef		mPreviousSelection;
	ImageViewRef		bgBox; 
	ImageViewRef		mBack; 

	TextViewRef		mTitle;
	TextViewRef		mSubtitle;

	TvMultiDisplayObjectRef	mTextBody;
	BugElementRef arrow; 
	bool arrowLit; 

	LocationViewRef	mOrbitalView;
	LocationViewRef mAerialView;
	LocationViewRef mLocalView;
	LocationViewRef mCurrentView;

	LocationTransitionRef mTransitions;

	LocationNodeRef mCurrentLocation;

	std::map<ButtonViewRef, LocationViewRef> mContentMap;

	bool mUseTransition;
	bool mIsInTransition;

	void checkCurrentLocation();

public:
	LocationView(Assets* assets, VideoViewRef video);
	~LocationView(void);
	
	virtual void	onAnimateIn();
	virtual void	onAnimateOut();
	void setCurrentData();

	virtual void	update(float dt);
	virtual void	prerender();
	virtual void	render();

	virtual void	setSelection(ci::Vec2i selection);
	virtual void	handleInput(INPUT_TYPE type);
    
#ifdef _WIN32
	virtual void	handleGamepad(XINPUT_GAMEPAD gamepad);
#endif

	void startTransition(LocationTransitionView::TransitionType type);
	void onTransitionListener(LocationTransitionView::TransitionEvent event);
	void onTransitionComplete();
};
