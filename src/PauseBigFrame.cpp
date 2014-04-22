#include "PauseBigFrame.h"

using namespace ci;
using namespace ci::app;

PauseBigFrame::PauseBigFrame(Assets* assets, float dur)
{
	duration = dur;

	video = ScrubVideoRef( new ScrubVideoObject(assets->getVideoByID("pause_video")) ); 
	video->stop(); 
	video->setCenter(ci::Vec3f(0.5f, 0.5f, 0)); 

	cta = ImageViewRef( new ImageView(assets->getTextureByID("pause_timelineCTA")) ); 
	cta->setCenter(ci::Vec3f(0.5f, 0.5f, 0)); 
	cta->setPosition(ci::Vec3f(0,205.0f,0)); 

	timecodeBG1 = ImageViewRef( new ImageView(assets->getTextureByID("buttonBG")) ); 
	timecodeBG1->setSize(ci::Vec3f(180.0f, 40.0f, 0)); 
	timecodeBG1->setPosition(ci::Vec3f(-887.0f/2.0f, -376.0f/2.0f, 0)); 
	//timecodeBG1->setAdditiveBlending(true);
	
	timecodeBG2 = ImageViewRef( new ImageView(assets->getTextureByID("buttonBG")) ); 
	timecodeBG2->setSize(ci::Vec3f(180.0f, 40.0f, 0)); 
	timecodeBG2->setPosition(ci::Vec3f(-887.0f/2.0f, -376.0f/2.0f, 0)); 
	timecodeBG2->setAdditiveBlending(true);
	timecodeBG2->setAlpha(0);

	timecode = TextViewRef( new TextView("00:00:00", assets->getFontByID("pause_timecode"), ci::ColorA(1.0f,1.0f,1.0f,1.0f)) );
	timecode->setCenter(ci::Vec3f(0.5f,0.5f,0.5f)); 
	timecode->setPosition(ci::Vec3f(-887.0f/2.0f + 180.0f/2.0f, -376.0f/2.0f + 40.0f/2.0f - 2.0f, 0)); 

	glow = ImageViewRef(new ImageView(assets->getTextureByID("pause_glow"))); 
	glow->setCenter(ci::Vec3f(0.5f, 0.5f, 0.5f)); 
	glow->setAlpha(0); 
	glow->setColor( ci::Vec3f(0,0.25f,1.0f) );
	glow->setAdditiveBlending(true);

	beaconIcon = ImageViewRef( new ImageView(assets->getTextureByID("pause_icon_social_7")) ); 
	beaconIcon->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	beaconIcon->setPosition( ci::Vec3f(-380.0f, 205.0f, 0) ); 

	beaconText = TextViewRef(new TextView("1", assets->getFontByID("pause_beacon"), ci::ColorA(0.66f, 0.59f, 0.3f, 1.0f)));
	beaconText->setCenter(ci::Vec3f(1.0f,0.5f,0)); 
	beaconText->setPosition( ci::Vec3f(-398.0f, 208.0f, 0) ); 
	numBeacons = 1;

	addChild( glow ); 
	addChild( video ); 
	addChild( timecodeBG2 ); 
	addChild( timecodeBG1 ); 
	addChild( timecode ); 
	addChild( cta ); 
	addChild( beaconText ); 
	addChild( beaconIcon ); 
}

void PauseBigFrame::setBeaconNum(int val)
{
	if(numBeacons == val) return; 

	numBeacons = val; 
	beaconText->setText( toString(numBeacons) );
}

void PauseBigFrame::setPercent(float val)
{
	//hack to keep it from going black on the first frame
	float o = 0; 
	if( val < 0.1f) o = 0.1f; 
	video->setTime( val * video->getDuration() + o ); 

	int time = (int)floor( val * duration ); 
	int hour = (time / 60 / 60) % 24;
    int min = (time / 60) % 60;
    int sec = time % 60;

	std::string s; 
	if( hour < 10 ) s+= "0";
	s+= toString(hour) + ":";
	if( min < 10 ) s += "0";
	s+= toString(min) + ":";
	if( sec < 10) s += "0";
	s +=  toString(sec);

	timecode->setText( s ); 
}
void PauseBigFrame::forceAnimateIn()
{
	video->setScale(ci::Vec3f(0.75f, 0.75f, 0.75)); 
	cta->setAlpha(0); 
	timecodeBG2->setPosition( ci::Vec3f(-887.0f/2.0f, -376.0f/2.0f, 0) ); 
	timecodeBG2->setAlpha( 0 ); 
	timecodeBG1->setAlpha( 0 ); 
	timecode->setAlpha( 0 ); 

	timeline().apply( video->getScale(), ci::Vec3f(1.0f, 1.0f, 1.0f), 0.3f, EaseOutQuint() ).delay(0.5f);
	timeline().apply( cta->getAlpha(), 1.0f, 0.3f, EaseOutQuint() ).delay(0.5f);

	timeline().apply( timecodeBG2->getPosition(), ci::Vec3f(-887.0f/2.0f + 5, -376.0f/2.0f + 5, 0), 0.3f, EaseOutQuint() ).delay(0.5f);
	timeline().apply( timecodeBG2->getAlpha(), 0.5f, 0.3f, EaseOutQuint() ).delay(0.5f);
	timeline().apply( timecodeBG1->getAlpha(), 0.8f, 0.3f, EaseOutQuint() ).delay(0.5f);
	timeline().apply( timecode->getAlpha(), 1.0f, 0.3f, EaseOutQuint() ).delay(0.5f);

	glow->setAlpha(1.0f);
	glow->setScale(ci::Vec3f(1.0f,1.0f,1.0f)); 
	timeline().apply( glow->getAlpha(), 0.0f, 1.0f, EaseOutQuint() ).delay(0.5f);
	timeline().apply( glow->getScale(), ci::Vec3f(4.0f,4.0f,4.0f), 1.0f, EaseOutQuint() ).delay(0.5f);
	
	beaconText->setAlpha(0.0f);
	timeline().apply( beaconText->getAlpha(), 1.0f, 0.25f, EaseOutQuint() ).delay(0.5f);

	beaconIcon->setAlpha(0.0f);
	timeline().apply( beaconIcon->getAlpha(), 1.0f, 0.25f, EaseOutQuint() ).delay(0.5f);
}

void PauseBigFrame:: selectView(bool val)
{
	if(val){
		timeline().apply( video->getScale(), ci::Vec3f(1.0f, 1.0f, 1.0f), 0.3f, EaseOutQuint() );
		timeline().apply( cta->getAlpha(), 1.0f, 0.3f, EaseOutQuint() );

		timeline().apply( timecodeBG2->getPosition(), ci::Vec3f(-887.0f/2.0f + 5, -376.0f/2.0f + 5, 0), 0.3f, EaseOutQuint() );
		timeline().apply( timecodeBG2->getAlpha(), 0.5f, 0.3f, EaseOutQuint() );
		timeline().apply( timecodeBG1->getAlpha(), 0.8f, 0.3f, EaseOutQuint() );
		timeline().apply( timecode->getAlpha(), 1.0f, 0.3f, EaseOutQuint() );

		glow->setAlpha(1.0f);
		glow->setScale(ci::Vec3f(1.0f,1.0f,1.0f)); 
		timeline().apply( glow->getAlpha(), 0.0f, 1.0f, EaseOutQuint() );
		timeline().apply( glow->getScale(), ci::Vec3f(4.0f,4.0f,4.0f), 1.0f, EaseOutQuint() );

		timeline().apply( beaconText->getAlpha(), 1.0f, 0.25f, EaseOutQuint() );
		timeline().apply( beaconIcon->getAlpha(), 1.0f, 0.25f, EaseOutQuint() );

	}else{
		timeline().apply( video->getScale(), ci::Vec3f(0.75f, 0.75f, 0.75), 0.3f, EaseOutQuint() );
		timeline().apply( cta->getAlpha(), 0.0f, 0.3f, EaseOutQuint() );

		timeline().apply( timecodeBG2->getPosition(), ci::Vec3f(-887.0f/2.0f, -376.0f/2.0f, 0), 0.3f, EaseOutQuint() );
		timeline().apply( timecodeBG2->getAlpha(), 0.0f, 0.3f, EaseOutQuint() );
		timeline().apply( timecodeBG1->getAlpha(), 0.0f, 0.3f, EaseOutQuint() );
		timeline().apply( timecode->getAlpha(), 0.0f, 0.3f, EaseOutQuint() );

		timeline().apply( beaconText->getAlpha(), 0.0f, 0.25f, EaseOutQuint() );
		timeline().apply( beaconIcon->getAlpha(), 0.0f, 0.25f, EaseOutQuint() );
	}
}

PauseBigFrame::~PauseBigFrame(void)
{

}
