#pragma once

#include "View.h"
#include "TextView.h"
#include "ImageView.h"

class LocationMapPOI : public View
{
	TextViewRef		mLocation;
	ImageViewRef	mArrow;
public:
	LocationMapPOI(Assets* assets, std::string title);
	~LocationMapPOI(void);

	//void render(ci::Vec3f camPos, ci::Vec3f lookAt);
};

typedef boost::shared_ptr<LocationMapPOI> LocationMapPOIRef;

