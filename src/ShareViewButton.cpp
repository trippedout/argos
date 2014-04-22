#include "ShareViewButton.h"

using namespace ci;
using namespace ci::app;

ShareViewButton::ShareViewButton(Assets* assets, std::string copy) : 
	ButtonView(assets),
	bg1( ImageViewRef( new ImageView(assets->getTextureByID("end_share_btn_bg")))),
	bg2( ImageViewRef( new ImageView(assets->getTextureByID("end_share_btn_bg")))),
	bg3( ImageViewRef( new ImageView(assets->getTextureByID("end_share_btn_bg")))),
	highlight( ImageViewRef( new ImageView(assets->getTextureByID("end_share_btn_bg_highlight"))))
{
	//todo use enum instead of std::string references. 
	bg1->setAlpha(0);
	bg2->setAlpha(1.f);
	bg3->setAlpha(0);

	highlight->setAlpha(0.f);

	/*bg1->setSize(ci::Vec3f(180.0f,40.0f,0));
	bg2->setSize(ci::Vec3f(180.0f,40.0f,0));
	bg3->setSize(ci::Vec3f(180.0f,40.0f,0));*/

	bg1->setAdditiveBlending(true);
	bg2->setAdditiveBlending(true);
	bg3->setAdditiveBlending(true);
	highlight->setAdditiveBlending(true);

	text = TextViewRef( new TextView(copy, assets->getFontByID("ARGOS_button"), ci::ColorA(1,1,1,1)) );
	text->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	text->setPosition(ci::Vec3f(bg1->getSize()->value().x * .5f, bg1->getSize()->value().y * .5f, 0.f));

	addChild(bg1); 
	addChild(bg2); 
	addChild(bg3); 
	addChild(highlight);
	addChild(text);  
}

void ShareViewButton::selectView(bool select)
{
	if(select == selected) return; 

	ButtonView::selectView(select); 

	float t = 0.3f; 

	if(selected){
		timeline().apply( bg1->getAlpha(), 1.f, t, EaseOutQuint() );
		timeline().apply( highlight->getAlpha(), 1.f, t, EaseOutQuint() );
		timeline().apply( bg3->getAlpha(), 1.f, t, EaseOutQuint() );
		
		timeline().apply( bg1->getPosition(), ci::Vec3f(-20,-20,0), t, EaseOutQuint() );
		timeline().apply( bg3->getPosition(), ci::Vec3f(20,20,0), t, EaseOutQuint() );
	}
	else{
		timeline().apply( bg1->getAlpha(), 0.0f, t*2, EaseOutQuint() );
		timeline().apply( highlight->getAlpha(), 0.f, t, EaseOutQuint() );
		timeline().apply( bg3->getAlpha(), 0.0f, t*2, EaseOutQuint() );
		
		timeline().apply( bg1->getPosition(), ci::Vec3f(0,0,0), t*2, EaseOutQuint() );
		timeline().apply( bg3->getPosition(), ci::Vec3f(0,0,0), t*2, EaseOutQuint() );
	}
}

ShareViewButton::~ShareViewButton(void)
{

}

TextViewRef ShareViewButton::getTextView()
{
	return this->text;
}