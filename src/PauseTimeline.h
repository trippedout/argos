#pragma once

#include "cinder/gl/Texture.h"
#include "ImageView.h"
#include "ButtonView.h"
#include "PauseBigFrame.h"
#include "cinder/Utilities.h"

#include "PauseSceneInfo.h"
#include "SoundController.h"

class PauseTimeline : public ButtonView
{

protected:
	
	float scrubVolume; 

	void snap(); 
	bool idle; 
	bool snapToInitialPosition; 
	float checkSceneTimer;
	float snapTimer; 
	float vel; 
	float percent; 
	float initialPercent;
	float videoX;
	float duration; 
	int selected;
	DisplayObjectContainerRef holder; 
	PauseBigFrame* frame;
	ImageViewRef bg; 
	ImageViewRef glow1; 
	ImageViewRef glow2; 
	ImageViewRef bigGlow; 
	
	FMOD::Channel* mScrubChannel;

	//ImageViewRef lightBeam; 

	typedef std::vector< ImageViewRef >::iterator ThumbIterator;
	std::vector< ImageViewRef > thumbs;
	std::vector< boost::shared_ptr<PauseSceneInfo> > info;

public:
	PauseTimeline(Assets* assets, float duration);
	~PauseTimeline(void);

	void checkData(); 

	virtual void update(float dt);

	virtual float getPercent(); 
	virtual void setPercent(float val); 
	virtual void setInitialPercent(float val);

	virtual void selectView(bool select);
	virtual void forceAnimateIn(); 
	
	virtual void onEnterView(); 
	virtual void onExitView(); 

	void move(float val);
	boost::shared_ptr<ImageView> getCurrentImageView(); 
};
