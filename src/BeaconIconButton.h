#pragma once

#include "ButtonView.h"
#include "ImageView.h"

class BeaconIconButton : public ButtonView
{
protected:
	ImageViewRef		mOn, mOff;
	int					mId;

public:
	BeaconIconButton(Assets* assets, int id);
	~BeaconIconButton(void);

	virtual void selectView(bool select);

	int getId() { return mId; };
};

