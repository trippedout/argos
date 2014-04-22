#include "BeaconAddNewView.h"

using namespace ci;
using namespace ci::app;

BeaconAddNewView::BeaconAddNewView(Assets* assets) : 
	BeaconSubView(assets),
	mIconChooser( BeaconIconChooserRef( new BeaconIconChooser(assets) ) ),
	mMessageChooser( BeaconMessageChooserRef( new BeaconMessageChooser(assets) ) ),
	mSubmitBtn( ButtonViewRef( new BeaconSubmitButton(assets) ) )
{
	assets->addTexture("beacon_add_header", new ci::gl::Texture( loadImage( loadAsset( "scenes/beacon/add_header.png" ) ) ) );
	assets->addTexture("beacon_add_icon_bg", new ci::gl::Texture( loadImage( loadAsset( "scenes/beacon/add_icon_bg.png" ) ) ) );

	mHeader = ImageViewRef( new ImageView( assets->getTextureByID("beacon_add_header") ) );
	this->addChild(mHeader);

	mAddIconBg = ImageViewRef( new ImageView( assets->getTextureByID("beacon_add_icon_bg") ) );
	mAddIconBg->setPosition(ci::Vec3f(0.f,mHeader->getSize()->value().y, 0.f));
	this->addChild(mAddIconBg);

	mIconChooser->setPosition(ci::Vec3f(25.f,mHeader->getSize()->value().y + 25.f, 0.f));
	this->addChild(mIconChooser);

	mMessageChooser->setPosition(ci::Vec3f(0.f, mHeader->getSize()->value().y + mAddIconBg->getSize()->value().y, 0.f));
	this->addChild(mMessageChooser);

	mSubmitBtn->setPosition(ci::Vec3f(0.f, mMessageChooser->getPosition()->value().y + mMessageChooser->getSize()->value().y, 0.f));
	this->addChild(mSubmitBtn);
}


BeaconAddNewView::~BeaconAddNewView(void)
{
	mHeader.reset();
	mAddIconBg.reset();
	mIconChooser.reset();
}

void BeaconAddNewView::handleInput(INPUT_TYPE type)
{
	ci::app::console() << "BeaconAddNewView::handleInput() " << type << std::endl;

	if(this->useSelection)
	{
		switch(type)
		{
		case SELECT:
			if(mIconChooser->useSelection)
			{
				mIconChooser->useSelection = false;
				mMessageChooser->setActive(true);
				mSubmitBtn->selectView(true);
			}
			else if(mMessageChooser->useSelection)
			{
				mIconChooser->setActive(false);
				mMessageChooser->setActive(false);
				mSubmitBtn->selectView(false);
				mCompletionSignal(CompletionType::COMPLETE);
			}
			return;

		case BACK:
			if(mIconChooser->useSelection)
			{
				ci::app::console() << "Icon Chooser active B" << std::endl;
				mIconChooser->setActive(false);
				mCompletionSignal(CompletionType::ESCAPED);
			}
			else if(mMessageChooser->useSelection)
			{
				mIconChooser->setActive(true);
				mMessageChooser->setActive(false);
			}
			return;
                
                
            default:
                break;
		}
		
		if(mMessageChooser->useSelection) mMessageChooser->handleInput(type);
		if(mIconChooser->useSelection) mIconChooser->handleInput(type);
	}
}

void BeaconAddNewView::setActive(bool active)
{
	this->useSelection = active;

	mIconChooser->setActive(active);
}