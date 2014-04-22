#include "DataViewOverview.h"

using namespace ci;
using namespace ci::app;

DataViewOverview::DataViewOverview(DataNodeRef dataNode, Assets* assets) : DataViewContentBase(assets),
	dataNode(dataNode),
	mText(TvMultiDisplayObjectRef( new TextViewMultiline( assets, dataNode->body) )),
	arrow(BugElementRef( new BugElement(assets->getTextureByID("data_item_arrow"), assets->getTextureByID("data_item_arrow_glow") ) ))
{
	mText->setPosition(ci::Vec3f(0,85,0));
	mText->project3D(true);
	this->addChild(mText);

	switch(dataNode->type)
	{
	case DataNode::DataType::ITEM:
		mImage = OvImageViewRef(new DataImage(assets->getTextureByID(dataNode->imageID), assets->getShaderByID("rotateSimple"), assets)); 
		break;
	default: 
		mImage = OvImageViewRef(new DataImage(assets->getTextureByID(dataNode->imageID), assets)); 
		break;
	}

	mImage->setPosition(ci::Vec3f(650.0f,0,0));
	arrow->setPosition(ci::Vec3f( 290.0f, 540.0f, 0));
	
	if(mText->scrollable()){
		arrow->show();
		addChild(arrow);
	}else{
		arrow->hide();
	}

	arrowLit = false;

	this->addChild(mImage);
}

DataViewOverview::~DataViewOverview(void)
{
}

void DataViewOverview::checkOniUnlock()
{
	// UNLOCK ONI SYMBOLS!?!
	//this is some real hacks... but i dont care :O
	if(dataNode->isOni() && dataNode->isOniLocked()){
		dataNode->setOniLocked(false); 

		if( dataNode->imageID == "data_HUGHES" ){	
			Overlay::getInstance().getView()->toast("DR. HUGHES"); 
		} else if( dataNode->imageID == "data_letter_head" ){
			Overlay::getInstance().getView()->toast("ONI ENCRYPTED FIELD RECORD"); 
		} else if( dataNode->imageID == "data_video_ONI_head" ){
				Overlay::getInstance().getView()->toast("DISCHARGE LETTER"); 
		}
	}
}

void DataViewOverview::onViewOpen()
{
	mImage->setAlpha(0);
	mImage->setPosition(ci::Vec3f(650.0f,0,-400.0f));
	timeline().apply(mImage->getPosition(), ci::Vec3f(650.0f,0,0), 1.0f, EaseOutQuint()).delay(0.1f);
	timeline().apply(mImage->getAlpha(), 1.0f, 0.2f, EaseOutQuint()).delay(0.1f);
	
	mText->setAlpha(0);
	mText->setPosition(ci::Vec3f(0,85,-400.0f));
	timeline().apply(mText->getPosition(), ci::Vec3f(0,85,0), 1.0f, EaseOutQuint()).delay(0.2f);
	timeline().apply(mText->getAlpha(), 1.0f, 0.2f, EaseOutQuint()).delay(0.2f);

	checkOniUnlock();
}

void DataViewOverview::onViewClose()
{
	timeline().apply(mImage->getPosition(), ci::Vec3f(650.0f,0,-300.0f), 1.0f, EaseInQuint()).delay(0.1f);
	timeline().apply(mImage->getAlpha(), 0.0f, 0.4f, EaseInQuint()).delay(0.1f);
	
	timeline().apply(mText->getPosition(), ci::Vec3f(0,85,-300.0f), 1.0f, EaseInQuint()).delay(0.15f);
	timeline().apply(mText->getAlpha(), 0.0f, 0.3f, EaseInQuint()).delay(0.15f);
}

void DataViewOverview::show(bool snap)
{
	mImage->setAlpha(0);
	mImage->setPosition(ci::Vec3f(650.0f,0,30.0f));
	timeline().apply(mImage->getPosition(), ci::Vec3f(650.0f,0,0), 0.2f, EaseOutQuint()).delay(0.1f);
	timeline().apply(mImage->getAlpha(), 1.0f, 0.2f, EaseOutQuint()).delay(0.1f);
	
	mText->setAlpha(0);
	mText->setPosition(ci::Vec3f(0,85,30.0f));
	timeline().apply(mText->getPosition(), ci::Vec3f(0,85,0), 0.2f, EaseOutQuint()).delay(0.2f);
	timeline().apply(mText->getAlpha(), 1.0f, 0.2f, EaseOutQuint()).delay(0.2f);

	checkOniUnlock();
}

void DataViewOverview::hide(bool snap)
{
	timeline().apply(mImage->getPosition(), ci::Vec3f(650.0f,0,30.0f), 0.2f, EaseInQuint()).delay(0.1f);
	timeline().apply(mImage->getAlpha(), 0.0f, 0.2f, EaseInQuint());

	timeline().apply(mText->getPosition(), ci::Vec3f(0,85,30.0f), 0.2f, EaseInQuint()).delay(0.2f);
	timeline().apply(mText->getAlpha(), 0.0f, 0.2f, EaseInQuint());
}

void DataViewOverview::hideImage()
{
	removeChild(mImage);
}

void DataViewOverview::handleInput(INPUT_TYPE type)
{
	//mText->handleInput(type);
}

#ifdef _WIN32
void DataViewOverview::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	if(mText->scrollable() ){
		int max = 32000; //ish? 
		ci::Vec2f left = ci::Vec2f((float)gamepad.sThumbLX/(float)max, gamepad.sThumbLY/(float)max ); 
		
		// dpad overrides the left stick. 
		int dpadUp = (int)(gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
		int dpadDown = (int)(gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
	
		if(  dpadUp > 0 ) {
			left.y = -0.75f; 
		} else if( dpadDown > 0 ) {
			left.y = 0.75f; 
		}

		if(!arrowLit && left.y > 0.25f || left.y < -0.25f){
			arrowLit = true; 
			arrow->glow();
		}
		mText->handleGamepad(gamepad);
	}else{
		if(mText->scrollable() && arrowLit){
			arrowLit = false;
			arrow->show(); 
		}
	}
}
#endif