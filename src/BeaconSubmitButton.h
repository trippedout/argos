#pragma once

#include "ButtonView.h"
#include "ImageView.h"

class BeaconSubmitButton : public ButtonView
{
	ImageViewRef	mSubmitOff, mSubmitOn;

public:
	BeaconSubmitButton(Assets* assets);
	~BeaconSubmitButton(void);

	virtual void selectView(bool select);
};

