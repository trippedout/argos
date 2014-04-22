#include "DataViewListIcon.h"

using namespace ci;
using namespace ci::app;

DataViewListIcon::DataViewListIcon(Assets* assets, IconType type, std::string copy) : ButtonView(assets)
{
	init(assets, type); 

	label = TextViewRef( new TextView(copy, assets->getFontByID("data_icon_label"), ci::ColorA(1,1,1,1)) );
	label->setCenter(ci::Vec3f(0,0.5f,0));
	label->setPosition(ci::Vec3f(90.0f, icon->getSize()->value().y/2.0f, 0)); 
	label->setAlpha(0);
	addChild(label); 
}

DataViewListIcon::DataViewListIcon(Assets* assets, IconType type) : ButtonView(assets)
{
	init(assets, type); 
}

void DataViewListIcon::init(Assets* assets, IconType type)
{
	std::string id = "data_icon_figure";

	switch(type)
	{
	case IconType::AUDIO:
		id = "data_icon_audio";
		break;
	case IconType::DOCUMENT:
		id = "data_icon_document";
		break;
	case IconType::MOVIE:
		id = "data_icon_video";
		break;
	case IconType::OBJECT3D:
		id = "data_icon_object3D";
		break;
	case IconType::FIGURE:
		id = "data_icon_figure";
		break;
	case IconType::CLOTHES:
		id = "data_icon_clothes";
		break;
	case IconType::BOOK:
		id = "data_icon_book";
		break;
	case IconType::OST:
		id = "data_icon_ost";
		break;
	case IconType::HELMET:
		id = "data_icon_helmet";
		break;
	case IconType::DOGTAG:
		id = "data_icon_dogtag";
		break;
	}

	icon = ImageViewRef( new ImageView(assets->getTextureByID(id)));
	icon->setAlpha(1.0f);

	icon2 = ImageViewRef( new ImageView(assets->getTextureByID(id)));
	icon2->setAlpha(0.4f);

	icon3 = ImageViewRef( new ImageView(assets->getTextureByID(id)));
	icon3->setAlpha(0);

	this->addChild(icon3);
	this->addChild(icon);
	this->addChild(icon2);
	this->selectView(false);
}

DataViewListIcon::~DataViewListIcon(void)
{

}

void DataViewListIcon::selectView(bool select)
{
	if(select){
		timeline().apply(icon->getAlpha(), 0.4f, .1f, EaseOutQuint());

		timeline().apply(icon2->getAlpha(), 1.0f, .1f, EaseOutQuint());
		timeline().apply(icon3->getAlpha(), 0.2f, .1f, EaseOutQuint());
		
		timeline().apply(icon2->getPosition(), ci::Vec3f(0,0,-10), .3f, EaseOutQuint());
		timeline().apply(icon3->getPosition(), ci::Vec3f(0,0,10), .3f, EaseOutQuint());

		if(label)label->setAlpha(1.0f);
	}
	else{
		timeline().apply(icon->getAlpha(), 0.0f, .3f, EaseOutQuint());
		timeline().apply(icon2->getAlpha(), 0.4f, .3f, EaseOutQuint());
		timeline().apply(icon3->getAlpha(), 0.0f, .3f, EaseOutQuint());
		
		timeline().apply(icon2->getPosition(), ci::Vec3f(0,0,0), .3f, EaseOutQuint());
		timeline().apply(icon3->getPosition(), ci::Vec3f(0,0,0), .3f, EaseOutQuint());

		if(label)label->setAlpha(0);
	}
}