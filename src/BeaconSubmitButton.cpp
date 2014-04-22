#include "BeaconSubmitButton.h"

using namespace ci;
using namespace ci::app;

BeaconSubmitButton::BeaconSubmitButton(Assets* assets) : ButtonView(assets)
{
	assets->addTexture("beacon_add_submit_off", new ci::gl::Texture( loadImage( loadAsset( "scenes/beacon/submit_off.png" ) ) ) );
	assets->addTexture("beacon_add_submit_on", new ci::gl::Texture( loadImage( loadAsset( "scenes/beacon/submit_on.png" ) ) ) );

	mSubmitOff = ImageViewRef( new ImageView( assets->getTextureByID("beacon_add_submit_off")));
	this->addChild(mSubmitOff);

	mSubmitOn = ImageViewRef( new ImageView( assets->getTextureByID("beacon_add_submit_on")));
	mSubmitOn->setAlpha(0.f);
	this->addChild(mSubmitOn);
}


BeaconSubmitButton::~BeaconSubmitButton(void)
{

}

void BeaconSubmitButton::selectView(bool select)
{
	if(select == selected) return; 

	ButtonView::selectView(select); 

	if(selected){
		mSubmitOff->setAlpha(0.f);
		mSubmitOn->setAlpha(1.f);
	}
	else{
		mSubmitOff->setAlpha(1.f);
		mSubmitOn->setAlpha(0.f);
	}
}