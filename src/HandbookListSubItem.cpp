#include "HandbookListSubItem.h"

using namespace ci;
using namespace ci::app;

int HandbookListSubItem::subItemNum = 2;

HandbookListSubItem::HandbookListSubItem(Assets* assets, std::string copy, std::string locationID, LocationNodeRef locationNode) : 
	ButtonView(assets),
	mLocationNode(locationNode),
	mLocked(false),
	locationID(locationID),
	bg1( ImageViewRef( new ImageView(assets->getTextureByID("hb_buttonBG")) )),
	bg2( ImageViewRef( new ImageView(assets->getTextureByID("hb_buttonBG")) )),
	bg3( ImageViewRef( new ImageView(assets->getTextureByID("hb_buttonBG")) )),
	text( TextViewRef( new TextView(copy, assets->getFontByID("handbook_listSubItem"), ci::ColorA(1.0f,1.0f,1.0f,1.0f)) )),
	lock( ImageViewRef( new ImageView(assets->getTextureByID("hb_lock")) )),
	textHighlight( TextViewRef( new TextView(copy, assets->getFontByID("handbook_listSubItem"), ci::ColorA(1.0f,1.0f,1.0f,1.0f)) ))
{
	init(); 
}

HandbookListSubItem::HandbookListSubItem(Assets* assets, std::string copy, DataNodeRef dataNode) : 
	ButtonView(assets),
	mDataNode(dataNode),
	mLocked(false),
	locationID(""),
	bg1( ImageViewRef( new ImageView(assets->getTextureByID("hb_buttonBG")) )),
	bg2( ImageViewRef( new ImageView(assets->getTextureByID("hb_buttonBG")) )),
	bg3( ImageViewRef( new ImageView(assets->getTextureByID("hb_buttonBG")) )),
	text( TextViewRef( new TextView(copy, assets->getFontByID("handbook_listSubItem"), ci::ColorA(1.0f,1.0f,1.0f,1.0f)) )),
	lock( ImageViewRef( new ImageView(assets->getTextureByID("hb_lock")) )),
	textHighlight( TextViewRef( new TextView(copy, assets->getFontByID("handbook_listSubItem"), ci::ColorA(1.0f,1.0f,1.0f,1.0f)) ))
{
	init(); 
}

void HandbookListSubItem::init()
{
	constellation = subItemNum;
	subItemNum++; 
	if(subItemNum >= 37) subItemNum = 2; 

	bg1->setAlpha(0.f);
	bg2->setAlpha(0.f);
	bg3->setAlpha(0.f);

	bg1->setSize(ci::Vec3f(485.f,32.f,0));
	bg2->setSize(ci::Vec3f(485.f,32.f,0));
	bg3->setSize(ci::Vec3f(485.f,32.f,0));

	bg1->setAdditiveBlending(true);
	bg2->setAdditiveBlending(true);
	bg3->setAdditiveBlending(true);

	text->setCenter(ci::Vec3f(0.f,0.5f,0)); 
	text->setPosition(ci::Vec3f(10.0f, 38.f/2.0f, 0));
	text->project3D(true);
	
	//lock
	lock->setCenter(ci::Vec3f(0.f,0.5f,0.f)); 
	lock->setPosition(ci::Vec3f(15.f + text->getSize()->value().x, 14.f, 0.f));
	lock->setAdditiveBlending(true);
	lock->project3D(true);

	textHighlight->setCenter(ci::Vec3f(0.f,0.5f,0)); 
	textHighlight->setPosition(ci::Vec3f(10.0f, 38.f/2.0f, 0));
	textHighlight->setAlpha(0.f);
	textHighlight->project3D(true);

	holder = DisplayObjectContainerRef( new DisplayObjectContainer() );
	addChild(holder); 

	holder->addChild(bg1); 
	holder->addChild(bg2); 
	holder->addChild(text); 
	holder->addChild(textHighlight);
	holder->addChild(bg3);  

	this->setSize(ci::Vec3f(485.f,38.f,0.f));
	holder->setAlpha(0.f);

	setLocked(true);
}

HandbookListSubItem::~HandbookListSubItem(void)
{
	bg1.reset();
	bg2.reset();
	bg3.reset();

	text.reset();
	textHighlight.reset();
}

void HandbookListSubItem::setLocked(bool val)
{
	mLocked = val;
	float t = 0.3f; 

	if(mLocked){
		timeline().apply( bg1->getColor(), ci::Vec3f(1.0f, 1.0f, 1.0f), t, EaseOutQuint() );
		timeline().apply( bg2->getColor(), ci::Vec3f(1.0f, 1.0f, 1.0f), t, EaseOutQuint() );
		timeline().apply( bg3->getColor(), ci::Vec3f(1.0f, 1.0f, 1.0f), t, EaseOutQuint() );
		holder->addChild(lock);

		text->setColor( ci::Vec3f(1.0f,1.0f,1.0f) ); 
		text->setAlpha(0.5f);

	}else{
		timeline().apply( bg1->getColor(), ci::Vec3f(0, 0.5f, 0.99f), t, EaseOutQuint() );
		timeline().apply( bg2->getColor(), ci::Vec3f(0, 0.5f, 0.99f), t, EaseOutQuint() );
		timeline().apply( bg3->getColor(), ci::Vec3f(0, 0.5f, 0.99f), t, EaseOutQuint() );
		holder->removeChild(lock);
		
		text->setAlpha(1.0f);
		text->setColor( ci::Vec3f(46.f/255.f,183.f/255.f,240.f/255.f) ); 
	}
}

void HandbookListSubItem::selectView(bool select)
{
	if(select == selected) return; 

	ButtonView::selectView(select); 

	float t = 0.3f; 

	if(selected)
	{
		textHighlight->setAlpha(1.f);
		text->setAlpha(0.f);

		timeline().apply( bg1->getAlpha(), mLocked? 0.1f : 0.5f, t, EaseOutQuint() );
		timeline().apply( bg2->getAlpha(), mLocked? 0.25f : 0.5f, t, EaseOutQuint() );
		timeline().apply( bg3->getAlpha(), mLocked? 0.1f : 0.5f, t, EaseOutQuint() );
		
		timeline().apply( bg1->getPosition(), ci::Vec3f(-2,-2,0), t, EaseOutQuint() );
		timeline().apply( bg3->getPosition(), ci::Vec3f(2,2,0), t, EaseOutQuint() );
	}
	else
	{
		textHighlight->setAlpha(0.f);
		text->setAlpha(mLocked? 0.5f : 1.0f);

		timeline().apply( bg1->getAlpha(), 0.0f, t*2, EaseOutQuint() );
		timeline().apply( bg2->getAlpha(), 0.0f, t*2, EaseOutQuint() );
		timeline().apply( bg3->getAlpha(), 0.0f, t*2, EaseOutQuint() );
		
		timeline().apply( bg1->getPosition(), ci::Vec3f(0,0,0), t*2, EaseOutQuint() );
		timeline().apply( bg3->getPosition(), ci::Vec3f(0,0,0), t*2, EaseOutQuint() );
	}
}


void HandbookListSubItem::onAnimateIn(float delay)
{
	holder->setAlpha(0.f);
	timeline().apply(holder->getAlpha(), 1.0f, 0.3f, EaseOutQuint()).delay(delay);

	holder->setPosition(ci::Vec3f(30.0f,0,0)); 
	timeline().apply(holder->getPosition(), ci::Vec3f(0,0,0), 0.5f, EaseOutQuint()).delay(delay);

	onAnimateInComplete();
}

void HandbookListSubItem::onAnimateIn()
{
	holder->setAlpha(1.f);
	onAnimateInComplete();
}

void HandbookListSubItem::onAnimateOut()
{
	timeline().apply(holder->getAlpha(), 0.0f, 0.0f, EaseOutQuint());
	onAnimateOutComplete();
}
