#pragma once

#include "InputView.h"
#include "HostButtonView.h"
#include "ImageView.h"
#include "TextView.h"

class HostView : public InputView
{
	typedef std::vector<HostButtonViewRef>::iterator HostButtonIter;
protected:
	TextViewRef                         mTitle;
	std::map<std::string, ci::Vec3f>	mAnimMap;
	std::vector<HostButtonViewRef>      buttons;
	ImageViewRef				mAddBack;

public:
	HostView(Assets* assets);
	
	virtual void onAnimateIn();
	virtual void onAnimateOut();

	virtual void handleInput(INPUT_TYPE type);
	virtual void playMovementSound(ci::Vec2i selection);

	~HostView(void);
};

