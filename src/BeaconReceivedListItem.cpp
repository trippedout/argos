#include "BeaconReceivedListItem.h"

using namespace ci;
using namespace ci::app;

BeaconReceivedListItem::BeaconReceivedListItem(Assets* assets, BeaconDataRef beaconData) : 
	ButtonView(assets),
	mBeaconData(beaconData),
	mIsOpen(false)
{	
	init(mBeaconData->userName, mBeaconData->message);
}

BeaconReceivedListItem::BeaconReceivedListItem(Assets* assets, std::string message) : 
	ButtonView(assets),
	mIsOpen(false)
{
	init(message);
}

void BeaconReceivedListItem::init(std::string user, std::string message)
{
	mBackgroundOff = ImageViewRef( new ImageView(mAssets->getTextureByID("received_bg_off")));
	this->addChild(mBackgroundOff);
	
	mBackgroundOn = ImageViewRef( new ImageView(mAssets->getTextureByID("received_bg_on")));
	mBackgroundOn->setAlpha(0.f);
	this->addChild(mBackgroundOn);

	mTextOff = TextViewRef( new TextView(user, mAssets->getFontByID("data_overview_body"), ci::ColorA(.2,.5,.8,1)));
	mTextOff->setCenter(ci::Vec3f(0,.5,0));
	mTextOff->setPosition(ci::Vec3f(15, mBackgroundOff->getSize()->value().y * 0.5f, 0));
	this->addChild(mTextOff);

	mTextOn = TextViewRef( new TextView(user, mAssets->getFontByID("data_overview_body"), ci::ColorA(1,1,1,1)));
	mTextOn->setCenter(ci::Vec3f(0,.5,0));
	mTextOn->setAlpha(0.f);
	mTextOn->setPosition(ci::Vec3f(15, mBackgroundOff->getSize()->value().y * 0.5f, 0));
	this->addChild(mTextOn);

	//set size so list knows
	this->setSize(mBackgroundOff->getSize()->value());

	//sub item
	mSubBackground = ImageViewRef( new ImageView(mAssets->getTextureByID("received_sub_bg")));
	mSubBackground->setPosition(ci::Vec3f(0.f, 50.f, 0.f));
	mSubBackground->setScale(ci::Vec3f(1.f,0.f,1.f));
	this->addChild(mSubBackground);


	mSubText = TextViewRef( new TextView(message, mAssets->getFontByID("data_overview_body"), ci::ColorA(1,1,1,1)));
	mSubText->setCenter(ci::Vec3f(0,.5,0));
	mSubText->setPosition(ci::Vec3f(45.f, 75.f, 0.f));
	mSubText->setAlpha(0.f);
	this->addChild(mSubText);
}

BeaconReceivedListItem::~BeaconReceivedListItem(void)
{
	mBackgroundOff.reset();
	mBackgroundOn.reset();
	mTextOff.reset();
	mTextOn.reset();
	mSubBackground.reset();
	mSubText.reset();
}

void BeaconReceivedListItem::selectView(bool select)
{
	if(select == selected) return; 

	ButtonView::selectView(select); 

//	float t = 0.3f; 

	if(selected){
		mBackgroundOn->setAlpha(1.f);
		mBackgroundOff->setAlpha(0.f);

		mTextOn->setAlpha(1.f);
		mTextOff->setAlpha(0.f);
	}
	else{
		mBackgroundOn->setAlpha(0.f);
		mBackgroundOff->setAlpha(1.f);

		mTextOn->setAlpha(0.f);
		mTextOff->setAlpha(1.f);
	}
}

void BeaconReceivedListItem::open()
{
	timeline().apply(mSubBackground->getScale(), ci::Vec3f(1.f,1.f,1.f), .35f, EaseInOutCubic());
	timeline().apply(mSubText->getAlpha(), 1.f, .35f)
		.delay(.1f);

	//mSubBackground->setScale(1.f);
	//mSubText->setAlpha(1.f);
	mIsOpen = true;
}

void BeaconReceivedListItem::close()
{
	timeline().apply(mSubBackground->getScale(), ci::Vec3f(1.f,0.f,1.f), .35f, EaseInOutCubic());
	timeline().apply(mSubText->getAlpha(), 0.f, .2f);

	//mSubBackground->setAlpha(0.f);
	//mSubText->setAlpha(0.f);

	mIsOpen = false;
}
