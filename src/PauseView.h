#pragma once

#include "InputView.h"

#include "ARGOSButtonView.h"
#include "PauseTimeline.h"
#include "VideoObject.h"
#include "VideoSnapshot.h"

#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"
#include "cinder/gl/GlslProg.h"

#include "cinder/gl/Texture.h"

class PauseView :
	public InputView
{
protected: 
	//static const int FBO_WIDTH = 1920, FBO_HEIGHT = 1080;
	//gl::Fbo	fbo;
	//ci::gl::GlslProgRef shader; 

	TextViewRef title; 
	TextViewRef subtitle; 
	VideoSnapshotRef snapshot; 
	VideoViewRef video; 
	ImageViewRef mAddBack;
	ImageViewRef bgBox; 

	ButtonViewRef handbook;
	ButtonViewRef prefs;
	boost::shared_ptr< PauseTimeline > pauseTimeline;

public:
	PauseView(Assets* assets, VideoViewRef vid, VideoSnapshotRef snap);
	~PauseView(void);
	
	//virtual void prerender(); 
	//virtual void render(); 

	virtual void onAnimateIn(Settings::ViewID from);
	virtual void onAnimateOut(Settings::ViewID to); 

#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamepad);
#endif
	virtual void handleInput(INPUT_TYPE type);
	
};

