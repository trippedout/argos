#include "TextButtonView.h"

using namespace ci;
using namespace ci::app;

TextButtonView::TextButtonView(Assets* assets, std::string buttonText) : ButtonView(assets)
{
	this->text1 = TextViewRef( new TextView(buttonText, assets->getFontByID(Assets::FileID::DEFAULT_FONT), ci::ColorA(1,1,1,1)));
	this->text2 = TextViewRef(new TextView(buttonText, assets->getFontByID(Assets::FileID::DEFAULT_FONT), ci::ColorA(0.16f, 0.67f,  0.88f,1)));

	this->addChild(text2);
	this->addChild(text1);
	
	this->text1->setAlpha(0); 
	this->text1->setScale(ci::Vec3f(0.66f,0.66f, 0.66f)); 
	this->text2->setScale(ci::Vec3f(0.66f,0.66f, 0.66f)); 
}

void TextButtonView::selectView(bool select)
{
	if(select == selected) return; 

	ButtonView::selectView(select); 

	float t = 0.3f; 

	if(selected){
		timeline().apply( text1->getAlpha(), 1.0f, t, EaseOutQuint() );
		timeline().apply( text1->getScale(), ci::Vec3f(1.0f,1.0f, 1.0f), t, EaseOutQuint() );
		timeline().apply( text2->getScale(),ci::Vec3f(1.0f,1.0f, 1.0f), t, EaseOutQuint() );
	}
	else{
		timeline().apply( text1->getAlpha(), 0.0f, t, EaseOutQuint() );

		timeline().apply( text1->getScale(), ci::Vec3f(0.66f,0.66f, 0.66f), t, EaseOutQuint() );
		timeline().apply( text2->getScale(), ci::Vec3f(0.66f,0.66f, 0.66f), t, EaseOutQuint() );
	}
}

TextButtonView::~TextButtonView(void)
{
	text1.reset();
	text2.reset();
	text1 = NULL;
	text2 = NULL;
}
