#pragma once

#include "View.h"
#include "ImageView.h"

class OverviewImage : public View
{
protected:
	ImageViewRef		mGuide;

	//eventually
	ImageViewRef		mImage;
	ImageViewRef		mBackground;

public:
	OverviewImage(Assets* assets);
	~OverviewImage(void);
};

