#include "HandbookTabView.h"

using namespace ci;
using namespace ci::app;

HandbookTabView::HandbookTabView(Assets* assets, std::string buttonText) : ButtonView(assets),
	lock( ImageViewRef( new ImageView(assets->getTextureByID("hb_lock")) )),
	text(TextViewRef( new TextView(buttonText, assets->getFontByID("handbook_tab"), ci::ColorA(1,1,1,1))))
{
	addChild(text);
	text->setAdditiveBlending(true);
	text->project3D(true);

	//lock
	lock->setCenter(ci::Vec3f(0.f,0.5f,0.f)); 
	lock->setPosition(ci::Vec3f(5.f + text->getSize()->value().x, text->getSize()->value().y/2.0f - 2.0f, 0.f));
	lock->setAdditiveBlending(true);
	lock->project3D(true);

	setLocked(false);
}

void HandbookTabView::selectView(bool select)
{
	float t = 0.3f; 

	if(select){
		timeline().apply( lock->getAlpha(), 1.0f, t, EaseOutQuint() );
		timeline().apply( text->getAlpha(), 1.0f, t, EaseOutQuint() );
	}
	else{
		timeline().apply( lock->getAlpha(), 0.5f, t, EaseOutQuint() );
		timeline().apply( text->getAlpha(), 0.5f, t, EaseOutQuint() );
	}
}

void HandbookTabView::setLocked(bool val)
{
	mLocked = val;

	if(mLocked){
		addChild(lock);
	}else{
		removeChild(lock);
	}
}

HandbookTabView::~HandbookTabView(void)
{
	text.reset();
	text = NULL;
}