#include "EndViewSmallButton.h"

using namespace ci;
using namespace ci::app;

EndViewSmallButton::EndViewSmallButton(Assets* assets, std::string copy) : 
	ButtonView(assets),
	bg1(ImageViewRef( new ImageView(assets->getTextureByID("buttonBG")))),
	bg2(ImageViewRef( new ImageView(assets->getTextureByID("buttonBG")))),
	bg3(ImageViewRef( new ImageView(assets->getTextureByID("buttonBG"))))
{
	bg1->setAlpha(0);
	bg2->setAlpha(0.5);
	bg3->setAlpha(0);

	bg1->setSize(ci::Vec3f(180.0f,25.0f,0));
	bg2->setSize(ci::Vec3f(180.0f,25.0f,0));
	bg3->setSize(ci::Vec3f(180.0f,25.0f,0));

	bg1->setAdditiveBlending(true);
	bg2->setAdditiveBlending(true);
	bg3->setAdditiveBlending(true);

	text = TextViewRef( new TextView(copy, assets->getFontByID("handbook_listItem"), ci::ColorA(1,1,1,1)) );
	text->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	text->setPosition(ci::Vec3f(90.0f, 13.0f, 0));
	text->setFontSize(18.f);
	//text->project3D(true);

	addChild(bg1); 
	addChild(bg2); 
	addChild(text); 
	addChild(bg3);  
}

void EndViewSmallButton::selectView(bool select)
{
	if(select == selected) return; 

	ButtonView::selectView(select); 

	float t = 0.3f; 

	if(selected){
		timeline().apply( bg1->getAlpha(), 0.5f, t, EaseOutQuint() );
		//timeline().apply( bg2->getAlpha(), 0.5f, t, EaseOutQuint() );
		timeline().apply( bg3->getAlpha(), 0.5f, t, EaseOutQuint() );
		
		timeline().apply( bg1->getPosition(), ci::Vec3f(-5,-5,0), t, EaseOutQuint() );
		timeline().apply( bg3->getPosition(), ci::Vec3f(5,5,0), t, EaseOutQuint() );
	}
	else{
		timeline().apply( bg1->getAlpha(), 0.0f, t*2, EaseOutQuint() );
		//timeline().apply( bg2->getAlpha(), 0.5f, t, EaseOutQuint() );
		timeline().apply( bg3->getAlpha(), 0.0f, t*2, EaseOutQuint() );
		
		timeline().apply( bg1->getPosition(), ci::Vec3f(0,0,0), t*2, EaseOutQuint() );
		timeline().apply( bg3->getPosition(), ci::Vec3f(0,0,0), t*2, EaseOutQuint() );
	}
}

EndViewSmallButton::~EndViewSmallButton(void)
{
	bg1.reset(); 
	bg2.reset(); 
	bg2.reset(); 
	text.reset(); 
}

TextViewRef EndViewSmallButton::getTextView()
{
	return this->text;
}