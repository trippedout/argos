#include "BeaconIconButton.h"

using namespace ci;
using namespace ci::app;

BeaconIconButton::BeaconIconButton(Assets* assets, int id) : 
	ButtonView(assets),
	mId(id),
	mOff( ImageViewRef( new ImageView( assets->getTextureByID( toString("beacon_icon_off_").append( toString(id) ) ) ) ) ),
	mOn( ImageViewRef( new ImageView( assets->getTextureByID( toString("beacon_icon_on_").append( toString(id) ) ) ) ) )
{
	this->addChild(mOff);

	mOn->setAlpha(0.f);
	this->addChild(mOn);
}


BeaconIconButton::~BeaconIconButton(void)
{
	mOff.reset();
	mOn.reset();
}

void BeaconIconButton::selectView(bool select)
{
	if(select)
	{
		mOff->setAlpha(0.f);
		mOn->setAlpha(1.f);
	}
	else
	{
		mOff->setAlpha(1.f);
		mOn->setAlpha(0.f);
	}
}