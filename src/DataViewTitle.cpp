#include "DataViewTitle.h"

using namespace ci;
using namespace ci::app;

DataViewTitle::DataViewTitle(boost::shared_ptr<DataNode> data,  Assets* assets) : View(assets)
{
	//set icon to default 
	mIcon	=	ImageViewRef(new ImageView(assets->getTextureByID(data->iconID)));
	mIcon->setAdditiveBlending(true);
	mIcon->project3D(true);
	this->addChild(mIcon);
	
	mTitle	=	TextViewRef(new TextView(data->title, assets->getFontByID("data_overview_head"), ci::ColorA(1,1,1,1)));
	mTitle->setPosition(ci::Vec3f(mIcon->getSize()->value().x + 5, -10.0f,0));
	mTitle->project3D(true);
	//mSubtitle->setFontSize(40);
	this->addChild(mTitle);

	mSubtitle	=	TextViewRef(new TextView(data->subtitle, assets->getFontByID("data_overview_subhead"), ci::ColorA(1,1,1,1)));
	mSubtitle->setPosition(ci::Vec3f(mIcon->getSize()->value().x + 5 + 9.0f,45,0));
	mSubtitle->project3D(true);
	this->addChild(mSubtitle);

	setSize( ci::Vec3f(mTitle->getSize()->value().x + mTitle->getPosition()->value().x,mIcon->getSize()->value().y,0) ); 
}

void DataViewTitle::slideHack(float t){
	mTitle->setPosition( ci::Vec3f(mIcon->getSize()->value().x + 5 + 80.0f, -10.0f,0) ); 
	timeline().appendTo(mTitle->getPosition(), ci::Vec3f(mIcon->getSize()->value().x + 5, -10.0f,0), t*1.5f, EaseOutQuint());
	
	mSubtitle->setPosition(ci::Vec3f(mIcon->getSize()->value().x + 5 + 9.0f + 150.0f, 45,0)); 
	timeline().appendTo(mSubtitle->getPosition(), ci::Vec3f(mIcon->getSize()->value().x + 5 + 9.0f,45,0), t*1.5f, EaseOutQuint()); 
}

DataViewTitle::~DataViewTitle(void)
{
}

void DataViewTitle::project3D(bool val)
{
	mIcon->project3D(val);
	mTitle->project3D(val);
	mSubtitle->project3D(val);
}
