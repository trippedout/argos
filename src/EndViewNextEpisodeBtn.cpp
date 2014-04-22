#include "EndViewNextEpisodeBtn.h"

EndViewNextEpisodeBtn::EndViewNextEpisodeBtn(Assets* assets) : 
	ButtonView(assets),
	mBg( ImageViewRef( new ImageView( assets->getTextureByID("end_next_episode_btn_bg") ) ) ),
	mText( TextViewRef( new TextView( "NEXT EPISODE", assets->getFontByID("ARGOS_button"), ci::ColorA(1,1,1,1) ) ) )
{
	//background
	mBg->setAdditiveBlending(true);
	this->addChild(mBg);

	//text
	mText->setCenter(ci::Vec3f(.5f,.5f,0.f));
	mText->setPosition(ci::Vec3f(mBg->getSize()->value().x * .5f, mBg->getSize()->value().y * .5f, 0.f));
	this->addChild(mText);

	this->setSize(mBg->getSize()->value());
}


EndViewNextEpisodeBtn::~EndViewNextEpisodeBtn(void)
{
	mBg.reset();
	mText.reset();
}

void EndViewNextEpisodeBtn::selectView(bool select)
{
	if(select)
		mBg->setAlpha(1.f);
	else		
		mBg->setAlpha(.5f);
}