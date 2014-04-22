#include "HandbookListItem.h"

using namespace ci;
using namespace ci::app;

HandbookListItem::HandbookListItem(Assets* assets, std::string title) : 
	ButtonView(assets),
	mText( TextViewRef( new TextView( title, assets->getFontByID("handbook_listItem"), ci::ColorA(1,1,1,1) ) ) ),
	mDivider( ImageViewRef( new ImageView( assets->getTextureByID("hb_list_item_divider") ) ) ),
	mIsShowingSublist(false),
	mLocked(false),
	bg1( ImageViewRef( new ImageView(assets->getTextureByID("hb_buttonBG")) )),
	bg2( ImageViewRef( new ImageView(assets->getTextureByID("hb_buttonBG")) )),
	bg3( ImageViewRef( new ImageView(assets->getTextureByID("hb_buttonBG")) )),
	lock( ImageViewRef( new ImageView(assets->getTextureByID("hb_lock")) )),
	mListIndex(0)
{

	bg1->setAlpha(0.f);
	bg2->setAlpha(0.f);
	bg3->setAlpha(0.f);

	float w = mDivider->getSize()->value().x;
	bg1->setSize(ci::Vec3f(w,36.f,0));
	bg2->setSize(ci::Vec3f(w,36.f,0));
	bg3->setSize(ci::Vec3f(w,36.f,0));

	bg1->setAdditiveBlending(true);
	bg2->setAdditiveBlending(true);
	bg3->setAdditiveBlending(true);

	//divider
	mDivider->setPosition(ci::Vec3f(0.f, 36.f, 0.f));
	mDivider->project3D(true);

	//text
	mText->setCenter(ci::Vec3f(0.f,0.5f,0.f));
	mText->setPosition(ci::Vec3f(15.f, 19.f, 0.f));
	mText->project3D(true);

	//lock
	lock->setCenter(ci::Vec3f(0.f,0.5f,0.f)); 
	lock->setPosition(ci::Vec3f(20.f + mText->getSize()->value().x, 16.f, 0.f));
	lock->setAdditiveBlending(true);
	lock->project3D(true);

	addChild(bg1); 
	addChild(bg2); 
	addChild(mText); 
	addChild(bg3);  
	this->addChild(mDivider);

	this->setSize(ci::Vec3f(580,38.f,0));

	setLocked(true);
}

HandbookListItem::~HandbookListItem(void)
{

}

void HandbookListItem::setLocked(bool val)
{
	mLocked = val;
	float t = 0.3f; 
	
	mText->setAlpha(mLocked ? 0.5f : 1.0f);

	if(mLocked){
		timeline().apply( bg1->getColor(), ci::Vec3f(1.0f, 1.0f, 1.0f), t, EaseOutQuint() );
		timeline().apply( bg2->getColor(), ci::Vec3f(1.0f, 1.0f, 1.0f), t, EaseOutQuint() );
		timeline().apply( bg3->getColor(), ci::Vec3f(1.0f, 1.0f, 1.0f), t, EaseOutQuint() );
		addChild(lock);
	}else{
		timeline().apply( bg1->getColor(), ci::Vec3f(0, 0.5f, 0.99f), t, EaseOutQuint() );
		timeline().apply( bg2->getColor(), ci::Vec3f(0, 0.5f, 0.99f), t, EaseOutQuint() );
		timeline().apply( bg3->getColor(), ci::Vec3f(0, 0.5f, 0.99f), t, EaseOutQuint() );
		removeChild(lock);
	}

}

void HandbookListItem::selectView(bool select)
{
	float t = 0.3f; 

	if(select)
	{
		timeline().apply( bg1->getAlpha(), mLocked? 0.1f : 0.5f, t, EaseOutQuint() );
		timeline().apply( bg2->getAlpha(), mLocked? 0.25f : 0.5f, t, EaseOutQuint() );
		timeline().apply( bg3->getAlpha(), mLocked? 0.1f : 0.5f, t, EaseOutQuint() );
		
		timeline().apply( bg1->getPosition(), ci::Vec3f(-2,-2,0), t, EaseOutQuint() );
		timeline().apply( bg3->getPosition(), ci::Vec3f(2,2,0), t, EaseOutQuint() );

		mText->setAlpha(1.0f);
	}
	else
	{
		timeline().apply( bg1->getAlpha(), 0.0f, t*2, EaseOutQuint() );
		timeline().apply( bg2->getAlpha(), 0.0f, t*2, EaseOutQuint() );
		timeline().apply( bg3->getAlpha(), 0.0f, t*2, EaseOutQuint() );
		
		timeline().apply( bg1->getPosition(), ci::Vec3f(0,0,0), t*2, EaseOutQuint() );
		timeline().apply( bg3->getPosition(), ci::Vec3f(0,0,0), t*2, EaseOutQuint() );

		mText->setAlpha(mLocked ? 0.5f : 1.0f);
	}

}

void HandbookListItem::onAnimateIn()
{
	mText->setAlpha(mLocked ? 0.5f : 1.0f);
}

void HandbookListItem::render()
{
	ButtonView::render();
}