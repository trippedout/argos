#pragma once

#include "InputView.h"

#include "DataFBO.h"
#include "DataArrowButton.h"
#include "Overlay.h"
#include "ImageView.h"
#include "VideoObject.h"
#include "VideoSnapshot.h"
#include "BackgroundController.h"

class DataView : public InputView
{

protected:
	ImageViewRef bgBox; 
	VideoViewRef video;
	VideoSnapshotRef videoSnapshot;
	ImageViewRef mAddBack;

	BackgroundController* mBgController;

	boost::shared_ptr< DataFBO > dataFBO;
	boost::shared_ptr< DataArrowButton > next;
	boost::shared_ptr< DataArrowButton > prev;

public:
	DataView(Assets* assets, VideoViewRef vid, VideoSnapshotRef videoSnapshot, BackgroundController* bgController);
	
	virtual void handleInput(INPUT_TYPE type);
#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamepad);
#endif
	virtual void prerender();

	virtual void onAnimateIn();
	virtual void onAnimateOut();

	~DataView(void);
};

