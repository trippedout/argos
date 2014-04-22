#include "DataArrowButton.h"

using namespace ci;
using namespace ci::app;

DataArrowButton::DataArrowButton(Assets* assets, bool next):DisplayObjectContainer()
{
	arrow = ImageViewRef( new ImageView(assets->getTextureByID("data_arrow")) ); 
	arrow2 = ImageViewRef( new ImageView(assets->getTextureByID("data_arrow")) );  
	arrow3 = ImageViewRef( new ImageView(assets->getTextureByID("data_arrow")) ); 
	
	float m;
	if(next){
		m = 1.0f;
		info = ImageViewRef( new ImageView(assets->getTextureByID("button_rb")) ); 
	}else{
		m = -1.0f;
		info = ImageViewRef( new ImageView(assets->getTextureByID("button_lb")) ); 
	}
	
	active = true; 
	
	arrow2->setAlpha(0.75f); 
	arrow2->setAlpha(0.1f); 
	arrow3->setAlpha(0.1f); 

	arrow->setAdditiveBlending(true); 
	arrow3->setAdditiveBlending(true); 
	arrow3->setAdditiveBlending(true); 

	info->setCenter(ci::Vec3f(0.5f,0.5f,0));
	arrow->setCenter(ci::Vec3f(0.5f,0.5f,0));
	arrow2->setCenter(ci::Vec3f(0.5f,0.5f,0));
	arrow3->setCenter(ci::Vec3f(0.5f,0.5f,0));

	arrow->setScale(ci::Vec3f(m,1.0f,1.0f));
	arrow2->setScale(ci::Vec3f(m,1.0f,1.0f));
	arrow3->setScale(ci::Vec3f(m,1.0f,1.0f));

	info->setPosition(ci::Vec3f(-35.0f*m,0,0));
	arrow->setPosition(ci::Vec3f(23.0f*m,0,0));
	arrow2->setPosition(ci::Vec3f(23.0f*m + 5*m, -5*m, 0));
	arrow3->setPosition(ci::Vec3f(23.0f*m - 5*m, 5*m, 0));
	
	addChild(arrow2); 
	addChild(arrow); 
	addChild(arrow3); 

	addChild(info); 
}

void DataArrowButton::pulse()
{
	if(!active) return; 

	timeline().removeTarget(arrow->getAlpha());
	timeline().removeTarget(arrow2->getAlpha());
	timeline().removeTarget(arrow3->getAlpha());

	timeline().apply(arrow->getAlpha(), 1.0f, 0.1f); 
	timeline().appendTo(arrow->getAlpha(), 0.75f, 0.25f, EaseOutQuint()); 

	timeline().apply(arrow2->getAlpha(), 0.5f, 0.1f); 
	timeline().appendTo(arrow2->getAlpha(), 0.1f, 0.1f); 

	timeline().apply(arrow3->getAlpha(), 0.5f, 0.1f); 
	timeline().appendTo(arrow3->getAlpha(), 0.1f, 0.1f); 
}

void DataArrowButton::show()
{
	if(active) return; 
	active = true; 
	
	timeline().removeTarget(info->getAlpha());
	timeline().removeTarget(arrow->getAlpha());
	timeline().removeTarget(arrow2->getAlpha());
	timeline().removeTarget(arrow3->getAlpha());

	timeline().apply(info->getAlpha(), 1.0f, 0.25f, EaseOutQuint()); 
	timeline().apply(arrow->getAlpha(), 0.75f, 0.5f, EaseOutQuint()); 
	timeline().apply(arrow2->getAlpha(), 0.1f, 1.0f, EaseOutQuint()); 
	timeline().apply(arrow3->getAlpha(), 0.1f, 1.0f, EaseOutQuint()); 
}

void DataArrowButton::hide()
{
	if(!active) return; 

	active = false; 

	timeline().appendTo(info->getAlpha(), 0.0f, 0.25f, EaseInQuint()); 
	timeline().appendTo(arrow->getAlpha(), 0.0f, 0.25f, EaseInQuint()); 
	timeline().appendTo(arrow2->getAlpha(), 0.0f, 0.5f, EaseInQuint()); 
	timeline().appendTo(arrow3->getAlpha(), 0.0f, 0.5f, EaseInQuint()); 
}

DataArrowButton::~DataArrowButton(void)
{
}
