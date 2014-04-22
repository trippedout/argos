#include "BeaconTabView.h"

BeaconTabView::BeaconTabView(Assets* assets, std::string copy) : 
	ButtonView(assets),
	bg_on( ImageViewRef( new ImageView( assets->getTextureByID("beacon_tab_bg_on") ) ) ),
	bg_off( ImageViewRef( new ImageView( assets->getTextureByID("beacon_tab_bg_off") ) ) ),
	text_on( TextViewRef( new TextView( copy, assets->getFontByID("ARGOS_button"), ci::ColorA(1,1,1,1) ) ) ),
	text_off( TextViewRef( new TextView( copy, assets->getFontByID("ARGOS_button"), ci::ColorA(41.f/255.f,171.f/255.f,226.f/255.f,1) ) ) )
{
	bg_on->setAlpha(0.f);

	bg_on->setAdditiveBlending(true);
	bg_off->setAdditiveBlending(true);

	text_on->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	text_on->setPosition(ci::Vec3f(100.f, 25.0f, 0)); 
	text_on->setAlpha(0.f);

	text_off->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	text_off->setPosition(ci::Vec3f(100.f, 25.0f, 0)); 

	addChild(bg_on); 
	addChild(bg_off); 
	addChild(text_on); 
	addChild(text_off);  
}

BeaconTabView::~BeaconTabView(void)
{
	bg_on.reset();
	bg_off.reset();
	text_on.reset();
	text_off.reset();
}

void BeaconTabView::selectView(bool select)
{
	if(select == selected) return; 

	ButtonView::selectView(select); 

	if(selected){
		bg_off->setAlpha(0.f);
		text_off->setAlpha(0.f);

		bg_on->setAlpha(1.f);
		text_on->setAlpha(1.f);
	}
	else{
		bg_off->setAlpha(1.f);
		text_off->setAlpha(1.f);

		bg_on->setAlpha(0.f);
		text_on->setAlpha(0.f);
	}
}
