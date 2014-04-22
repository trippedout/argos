#pragma once

#include "View.h"

#include "ImageView.h"


class LocationLocalBgRingsView : public View
{
protected:
	ImageViewRef		mNsew;
	ImageViewRef		mInnerCircle;
	ImageViewRef		mInnerShapes;
	ImageViewRef		mOuterCircle;
	ImageViewRef		mStaticCircles;

	float				mRotation;

public:
	LocationLocalBgRingsView(Assets* assets);
	~LocationLocalBgRingsView(void);

	virtual void update(float dt);

	void setCameraRotation(float rotation);

};