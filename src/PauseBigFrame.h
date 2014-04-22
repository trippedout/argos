#pragma once
#include "DisplayObjectContainer.h"
#include "TextView.h"
#include "ScrubVideoObject.h"
#include "ImageView.h"
#include "Assets.h"


class PauseBigFrame : public DisplayObjectContainer
{
protected: 
	float time, duration; 

	ScrubVideoRef video;
	ImageViewRef cta; 
	ImageViewRef timecodeBG1;
	ImageViewRef timecodeBG2;
	ImageViewRef glow;

	ImageViewRef beaconIcon;
	TextViewRef beaconText; 
	int numBeacons; 

	TextViewRef timecode; 


public:
	PauseBigFrame(Assets* assets, float dur);
	~PauseBigFrame(void);

	void setPercent(float val); 
	void selectView(bool val); 
	void forceAnimateIn(); 

	void setBeaconNum(int val);
};

