#pragma once

#include "InputView.h"

#include "BeaconTabView.h"
#include "ImageView.h"
#include "TextView.h"
#include "VideoObject.h"

#include "BeaconReceivedView.h"
#include "BeaconAddNewView.h"

class BeaconView : public InputView
{
	typedef boost::shared_ptr<BeaconSubView> BeaconSubViewRef;
protected:
	bool	mIsShowing;
	bool	mIsComingFromVideo;

	ButtonViewRef					mReceivedTab;
	ButtonViewRef					mCreateTab;

	BeaconSubViewRef			mBeaconReceivedView;
	BeaconSubViewRef			mBeaconAddNewView;
	BeaconSubViewRef			mCurrentView;

	ImageViewRef					mSelectBack;

	std::map<ButtonViewRef, BeaconSubViewRef> mContentMap;

	VideoViewRef					mVideo;

public:
	BeaconView(Assets* assets, VideoViewRef video);
	~BeaconView(void);

	virtual void moveSelection(ci::Vec2i dir);
	virtual void handleInput(INPUT_TYPE type);

	virtual void render();
	virtual void onAnimateIn();
	virtual void onAnimateOut();

	void renderVideo();
		
	//callbacks
	void onViewComplete(BeaconSubView::CompletionType type);

	//overrides
	void playMovementSound(ci::Vec2i selection);

	//setters and getters
	void setComingFromVideo(bool fromVideo) { mIsComingFromVideo = fromVideo; }
	void setActive(bool active);
	
	bool getIsShowing();
};

