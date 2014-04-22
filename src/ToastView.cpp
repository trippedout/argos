#include "ToastView.h"

using namespace ci;
using namespace ci::app;

ToastView::ToastView(Assets* assets):View(assets)
{
	data = assets->getData(); 

	toasting = false;
	current = 0; 
	
	//times of the video where the toast animations are. 
	times.push_back( 0 ); 
	times.push_back( 2.0f + 2.0f / 30.0f ); 
	times.push_back( 3.0f + 26.0f / 30.0f ); 
	times.push_back( 5.0f + 27.0f / 30.0f ); 

	ci::Vec3f center = ci::Vec3f(1920.0f/2.0f, 1080.0f/2.90f, 0);

	video = ScrubVideoRef( new ScrubVideoObject(assets->getVideoByID("ONI_toast") ) ); 
	video->setScale( ci::Vec3f(1.25f,1.25f,1.25f) ); 
	video->setCenter( ci::Vec3f(0.5f, 0.5f, 0.5f) );
	video->setPosition( center ); 
	video->setAlpha(0);
	video->setAdditiveBlending(true);


	text = TextViewRef( new TextView("this is some text", assets->getFontByID("toast_text"), ci::ColorA(1,1,1,1)) );
	text->setCenter( ci::Vec3f(0.5f, 0.5f, 0.5f) );
	text->setPosition( center + ci::Vec3f(0,313.0f,0) ); 
	text->setAlpha(0);

	textBG = ImageViewRef( new ImageView(assets->getTextureByID("toast_textBG")) );
	textBG->setCenter( ci::Vec3f(0.5f, 0.5f, 0.5f) );
	textBG->setPosition( center + ci::Vec3f(0,310.0f,0) ); 
	textBG->setAlpha(0);

	glow = ImageViewRef( new ImageView(assets->getTextureByID("toast_glow")) );
	glow->setScale(ci::Vec3f(2.0f, 2.0f,0));
	glow->setCenter( ci::Vec3f(0.5f, 0.5f, 0.5f) );
	glow->setPosition( center ); 
	glow->setAdditiveBlending(true);
	glow->setAlpha(0);

	duration = video->getVideo()->getDuration();

	bgBox = ImageViewRef( new ImageView(assets->getTextureByID("bgBox")) );
	bgBox->setScale(ci::Vec3f(1920.0f/bgBox->getSize()->value().x, 1080.0f/bgBox->getSize()->value().y,0));
	bgBox->setAlpha(0);

}

void ToastView::update(float dt)
{
	View::update(dt);

	if(toasting){
		int size = times.size(); 
		float t = duration;

		if( current < size-1 ){
			t = times[current+1]; 
		}

		if(video->getTime() >= t - 2/30.0f){
			video->stop();
			endToast(); 
		}
	}
}

void ToastView::endToast()
{
	toasting = false; 
	
	timeline().apply( video->getAlpha(), 0.0f, 0.5f, EaseInQuint() ).delay(2.0f);

	timeline().apply( textBG->getAlpha(), 0.0f, 0.5f, EaseInQuint() ).delay(2.5f);
	timeline().apply( text->getAlpha(), 0.0f, 0.5f, EaseInQuint() ).delay(2.5f);

	timeline().apply( bgBox->getAlpha(), 0.0f, 1.0f, EaseInQuint() ).delay(2.0f)
		.finishFn(std::bind(&ToastView::hideSelf,this));

}

void ToastView::hideSelf()
{
	if(contains(bgBox)) removeChild(bgBox); 
	if(contains(video)) removeChild(video);

	if(contains(glow)) removeChild(glow);
	if(contains(textBG)) removeChild(textBG);
	if(contains(text)) removeChild(text);
}

void ToastView::toast(std::string copy)
{
	addChild(bgBox); 
	addChild(video); 
	addChild(textBG); 
	addChild(text); 
	addChild(glow); 

	bgBox->setAlpha(0.0f);
	timeline().apply( bgBox->getAlpha(), 0.75f, 0.5f, EaseOutQuint() );

	glow->setAlpha(0.0f);
	timeline().apply( glow->getAlpha(), 1.0f, 0.1f, EaseOutQuint() );
	timeline().appendTo( glow->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ).delay(0.1f);

	textBG->setAlpha(0.0f);
	timeline().apply( textBG->getAlpha(), 1.0f, 0.5f, EaseOutQuint() ).delay(0.5f);
	text->setAlpha(0.0f);
	timeline().apply( text->getAlpha(), 1.0f, 0.5f, EaseOutQuint() ).delay(0.5f);

	text->setText( "ONI SYMBOL DETECTED: " + copy ); 

	video->setAlpha(1.0f);

	//TODO: glow animation + textfield
	int size = times.size(); 

	//IMPORTANT: unlock the oni symbol BEFORE toasting!
	DataController::UnlockedInfo unlocked = data->getUnlocked();
	current = unlocked.onis-1;
	if(current < 0) current = 0;
	if(current >= size) current = size-1;

	toasting = true; 

	video->setTime(times[current]); 
	video->play(); 
}

ToastView::~ToastView(void)
{
}
