#include "PauseTimeline.h"

using namespace ci;
using namespace ci::app;

PauseTimeline::PauseTimeline(Assets* assets, float dur):ButtonView(assets),
	scrubVolume(0)
{
	// small: 417 x 175 big:887 x 376
	vel = 0;
	duration = dur; 
	holder = DisplayObjectContainerRef( new DisplayObjectContainer());

	/*
	lightBeam = ImageViewRef( new ImageView( assets->getTextureByID("data_lightPulse") ) );
	lightBeam->setCenter(ci::Vec3f(0.5f, 0.0f, 0.0f)); 
	lightBeam->setScale(ci::Vec3f(1.0f, 2.0f, 1.0f));
	lightBeam->setRotation(ci::Vec3f(0,0,90.0f));
	lightBeam->setPosition(ci::Vec3f(1920.0f, 0, 0));
	lightBeam->setAlpha(0.1f);
	lightBeam->setAdditiveBlending(true);
	*/

	checkSceneTimer = 0;

	std::string n; 
	videoX = 1400.0f;

	snapToInitialPosition = false; 
	snapTimer = 0;

	for(int i=0; i<40; i++){
		n = ""; 
		if(i<10) n+= "00";
		else if(i<100) n+= "0";
		n += cinder::toString(i);
		boost::shared_ptr<ImageView> image = boost::shared_ptr<ImageView>( new ImageView( assets->getTextureByID("thumbs_00" + n) ) ); 
		
		image->setPosition(ci::Vec3f(videoX - 100.0f + (float)417*i, 0, 0)); 
		image->setCenter(ci::Vec3f(0.5f, 0.5f, 0)); 
		
		thumbs.push_back(image); 
		holder->addChild(image);
	}

	std::vector< boost::shared_ptr<Scene> > scenes = *assets->getData()->getScenes();
	int size = scenes.size();
	boost::shared_ptr<PauseSceneInfo> psInfo;

	for( int i=0; i<size; i++){
		psInfo = boost::shared_ptr<PauseSceneInfo>( new PauseSceneInfo( assets, scenes[i]) ); 
		psInfo->setPosition(ci::Vec3f(videoX  + 417.0f * 40.0f * (scenes[i]->getStart() / duration) - 100.0f + (417.0f/2.0f), -64.0f - 175.0f/2.0f , 0)); 
		
		info.push_back( psInfo ); 
		holder->addChild(psInfo);

		//spread em out of theyre too close. 
		if( i > 0 && abs( info[i]->getPosition()->value().x - info[i-1]->getPosition()->value().x) < 200.0f ){
			info[i-1]->hideBeaconText();
		}
		
	}

	//std::vector< boost::shared_ptr<PauseSceneInfo> > info

	bg = ImageViewRef( new ImageView(assets->getTextureByID("pause_timeline_bg")));
	glow1 = ImageViewRef( new ImageView(assets->getTextureByID("pause_beam"))); 
	glow2 = ImageViewRef( new ImageView(assets->getTextureByID("pause_beam")));
	bigGlow = ImageViewRef( new ImageView(assets->getTextureByID("pause_beam")));

	glow1->setAdditiveBlending(true);
	glow1->setPosition(ci::Vec3f(1920.0f, -bg->getSize()->value().y/2, 0));
	glow1->setCenter(ci::Vec3f(1.0f, 1.0f, 0)); 
	glow1->setScale(ci::Vec3f(1.5f,1.0f,1.0f));

	glow2->setAdditiveBlending(true); 
	glow2->setPosition(ci::Vec3f(1920.0f, bg->getSize()->value().y/2, 0)); 
	glow2->setCenter(ci::Vec3f(1.0f, 1.0f, 0)); 
	glow2->setScale(ci::Vec3f(1.5f,-1.0f, 0)); 

	bigGlow->setAdditiveBlending(true);
	bigGlow->setPosition(ci::Vec3f(1920.0f, -bg->getSize()->value().y/2, 0));
	bigGlow->setScale(ci::Vec3f(2.0f,2.0f,1.0f));
	bigGlow->setCenter(ci::Vec3f(1.0f, 1.0f, 0)); 
	bigGlow->setColor( ci::Vec3f(0,0.25f,1.0f) );

	bg->setPosition(ci::Vec3f(0, 0, 0)); 
	bg->setCenter(ci::Vec3f(0, 0.5f, 0)); 
	 
	idle = true; 
	selected = false; 
	percent = 0; 
	initialPercent = 0; 
	
	frame = new PauseBigFrame(assets, duration); 
	frame->setPosition(ci::Vec3f(videoX, 0, 0)); 
	frame->setBeaconNum( info[0]->getNumBeacons() ); 

	addChild(glow1); 
	addChild(glow2); 
	addChild(bigGlow); 
	addChild(bg); 
	addChild(holder); 
	addChild(DisplayObjectRef(frame));

	//addChild(lightBeam);
}

void PauseTimeline::onEnterView()
{
	SoundController::getInstance().playSound(SoundController::SCRUB_LOOP, &mScrubChannel);
	scrubVolume=0;
	mScrubChannel->setVolume(scrubVolume);
}

void PauseTimeline::onExitView()
{
	//check timer and kill
	bool* isPlaying = new bool;

	mScrubChannel->isPlaying(isPlaying);

	if(*isPlaying) mScrubChannel->stop();
	delete isPlaying;
} 

void PauseTimeline::checkData()
{
	int size = info.size(); 

	for( int i=0; i<size; i++){
		info[i]->checkData(); 
	}
}

void PauseTimeline::forceAnimateIn()
{
	selected = true; 
	frame->forceAnimateIn(); 

	bigGlow->setScale(ci::Vec3f(1.0f,0.0f,1.0f)); 
	bigGlow->setAlpha(0.5f); 
	glow1->setAlpha(0.5f); 
	glow2->setAlpha(0.5f); 
	//lightBeam->setAlpha(0.1f); 

	timeline().apply( bigGlow->getScale(), ci::Vec3f(2.0f,2.5f,1.0f), 0.5f, EaseOutQuint() ).delay(0.5f);
	timeline().apply( bigGlow->getAlpha(), 1.0f, 0.5f, EaseOutQuint() ).delay(0.5f);
	timeline().appendTo( bigGlow->getAlpha(), 0.0f, 0.5f, EaseInQuint() ).delay(0.5f);
	timeline().apply( glow1->getAlpha(), 1.0f, 0.7f, EaseOutQuint() ).delay(0.5f);
	timeline().apply( glow2->getAlpha(), 1.0f, 0.7f, EaseOutQuint() ).delay(0.5f);
	//timeline().apply( lightBeam->getAlpha(), 0.5f, 1.0f, EaseOutQuint() );
}

void PauseTimeline::selectView(bool select)
{
	if(selected == select) return; 
	selected = select;

	frame->selectView(selected); 
	vel =0; 

	if(selected){
		//show beams
		timeline().apply( bigGlow->getScale(), ci::Vec3f(2.0f,2.5f,1.0f), 0.5f, EaseOutQuint() );

		timeline().apply( bigGlow->getAlpha(), 1.0f, 0.5f, EaseOutQuint() );
		timeline().appendTo( bigGlow->getAlpha(), 0.0f, 0.5f, EaseInQuint() );

		timeline().apply( glow1->getAlpha(), 1.0f, 0.7f, EaseOutQuint() );
		timeline().apply( glow2->getAlpha(), 1.0f, 0.7f, EaseOutQuint() );
		
		//timeline().apply( lightBeam->getAlpha(), 0.5f, 1.0f, EaseOutQuint() );
	}
	else{
		//hide beams
		timeline().apply( bigGlow->getScale(), ci::Vec3f(1.0f,0.0f,1.0f), 0.5f, EaseOutQuint() );
		timeline().apply( bigGlow->getAlpha(), 0.0f, 0.5f, EaseInQuint() );

		timeline().apply( glow1->getAlpha(), 0.5f, 0.3f, EaseOutQuint() );
		timeline().apply( glow2->getAlpha(), 0.5f, 0.3f, EaseOutQuint() );

		//timeline().apply( lightBeam->getAlpha(), 0.1f, 0.3f, EaseOutQuint() );
	}
}

void PauseTimeline::setPercent(float val)
{
	float w = 417 * 39;
	float max = -w;
	percent = val; 

	frame->setPercent(percent); 
	holder->setPosition(ci::Vec3f(max * val, 0, 0)); 
}

void PauseTimeline::setInitialPercent(float val)
{
	initialPercent = val;
	setPercent(val); 
}

float PauseTimeline::getPercent()
{
	return percent; 
}

void PauseTimeline::update(float dt)
{
	DisplayObjectContainer::update(dt);
	

	if(idle){
		scrubVolume-=dt*10.0f; 
		if(scrubVolume<0) scrubVolume=0;
	}else{
		scrubVolume+=dt*10.0f; 
		if(scrubVolume>1.0f) scrubVolume=1.0f;
	}
	if(mScrubChannel) mScrubChannel->setVolume(scrubVolume);


	float diff = abs( percent - initialPercent );

	if( diff < 0.01f && snapToInitialPosition ){
		snapToInitialPosition = false; 
		snapTimer = 0.5f; 
		setPercent(initialPercent); 

		frame->selectView(true); 
	}

	//if snapping, dont move the timeline. 
	if( snapTimer > 0 ) {
		snapTimer-=dt;
		return; 
	}
	
	float w = 417 * 39;
	float max = -w;

	ci::Vec3f pos = holder->getPosition()->value() + ci::Vec3f(vel * dt, 0, 0);
	if(pos.x > 0) pos.x = 0; 
	if(pos.x < max) pos.x = max;

	holder->setPosition(pos); 
	
	percent = pos.x /max; 
	frame->setPercent(percent); 

	if( diff > 0.025f ){
		snapToInitialPosition = true; 
	}

	if(!idle){
		
		float currentTime = percent * duration;

		int size = info.size(); 
		

		for( int i=1; i<size; i++){
			if( info[i]->getSceneTime() > currentTime ){
				frame->setBeaconNum( info[i-1]->getNumBeacons() ); 
				return; 
			}
		}
		
		frame->setBeaconNum( info.back()->getNumBeacons() ); 
	}

}

void PauseTimeline::snap()
{

}

void PauseTimeline::move(float val)
{	
	//swap directions, it feels better this way. 
	val *= -1;

	if( abs(val) < 0.3 ) {
		vel = 0; 
		if(!idle){
			idle = true; 
			snap(); 
			timeline().apply( frame->getPosition(), ci::Vec3f(videoX, 0, 0), 0.3f, EaseOutQuint() );
		}
	}
	else{
		idle = false; 
		vel = pow(val, 2) * 2000.0f; 
		if( val < 0) vel *= -1; 

		timeline().apply( frame->getPosition(), ci::Vec3f(videoX - 40 * val, 0, 0), 1.5f, EaseOutQuint() );
	}
}

boost::shared_ptr<ImageView> PauseTimeline::getCurrentImageView()
{
	return thumbs[selected];
}


PauseTimeline::~PauseTimeline(void)
{
}
