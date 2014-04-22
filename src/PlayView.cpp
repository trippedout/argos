#include "PlayView.h"

PlayView::PlayView(Assets* assets, VideoViewRef vid) : InputView(assets)
{
	exitingToEnd = false; 
	video = vid; 
	video->stop();

	credits = 18*60.0f + 58.0f;
	videoDuration = video->getVideo()->getDuration();

	addChild(video); 
	
	frameSeekDelay = 0;
	inputDelay = 0;
	fastforwarding = false; 
	rewinding = false;

	rate  = 0; 
	seekSpeed = 300.0f; 
}

PlayView::~PlayView(void)
{

}

void PlayView::update(float dt)
{
	InputView::update(dt); 

	if(inputDelay >= 0) inputDelay-=dt;
	if(frameSeekDelay >= 0 ) frameSeekDelay-=dt;

	float time = video->getVideo()->getCurrentTime();
	float t = credits;
	if( mAssets->getData()->getEpisodeComplete() ) t = videoDuration*0.99f; 

	if(!exitingToEnd && time >= t ){
		exitingToEnd = true; 
		signal(this, Settings::ViewID::END);
		mAssets->getData()->setEpisodeComplete(true);
	}

	if( rewinding || fastforwarding ){
		//ci::app::console() << "PlayView::update() stopping video" << std::endl;
		if( video->getVideo()->isPlaying() ) video->stop(); 

		float m = rewinding ? -1 : 1;

		if(frameSeekDelay <0){
			frameSeekDelay = 1/12.0f;
			video->getVideo()->seekToTime( time + dt * m + seekSpeed * dt * m * rate ); 
		}
	}

	if( !video->getVideo()->isPlaying() && !rewinding && !fastforwarding  ){
		ci::app::console() << "PlayView::update() playing video" << std::endl;
		video->play(); 
	}
}

#ifdef _WIN32
void PlayView::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	
	int left = (int)gamepad.bLeftTrigger;
	int right = (int)gamepad.bRightTrigger;

	if( left >= 100 && !fastforwarding ){
		if(inputDelay < 0){
			ci::app::console() << "PlayView::handleGamepad() rewinding" << std::endl;
			rewinding = true; 
			inputDelay = 0.5f;
		}
	}else if( left < 100 && rewinding ){
		rewinding = false; 
		inputDelay = 0.5f;
	}
	
	if( right >= 100 && !rewinding){
		if(inputDelay < 0){
			ci::app::console() << "PlayView::handleGamepad() fastforwarding" << std::endl;
			fastforwarding = true; 
			inputDelay = 0.5f;
		}
	}else if( right < 100 && fastforwarding){
		fastforwarding = false; 
		inputDelay = 0.5f;
	}

	if(rewinding) rate = (left-100.0f)/155.0f;
	if(fastforwarding) rate = (right-100.0f)/155.0f;

	if( !fastforwarding && !rewinding ) rate = 0;
}
#endif

void PlayView::handleInput(INPUT_TYPE type)
{
	if(exitingToEnd) return; 
	
	float time = video->getTime(); 

	switch(type)
	{
	case SELECT:
		video->stop(); 
		signal(this, Settings::ViewID::PAUSE);
		break;

	case PAUSE:
		video->stop();
		signal(this, Settings::ViewID::PAUSE);
		break;

	case START:
		video->stop(); 
		signal(this, Settings::ViewID::PAUSE);
		break;
	
	case BACK:
		video->stop(); 
		signal(this, Settings::ViewID::BACK);
		break;

	case PREVIOUS:
		video->prevScene(); 
		break;

	case NEXT:
		video->nextScene(); 
		break;

	//voice specific
	case PRESSED_X:
	case HANDBOOK:
		video->stop(); 
		signal(this, Settings::ViewID::HANDBOOK);
		break;

	case DPAD_UP:
	case VOICE_UP:
	case BEACON:
		video->stop(); 
		signal(this, Settings::ViewID::BEACON);
		break;

	case DPAD_DOWN:
	case VOICE_DOWN:
	case DATA:
		SoundController::getInstance().playSound(SoundController::DATA_EXIT_MOVIE);
		video->stop(); 
		signal(this, Settings::ViewID::DATA);
		break;

	case DPAD_RIGHT:
	case VOICE_RIGHT:
	case LOCATION:
		video->stop(); 
		signal(this, Settings::ViewID::LOCATION);
		break;

	case DPAD_LEFT:
	case VOICE_LEFT:
		//hack, only open engage during time when theyre using the keypad
		if(time > 18*60.0f + 2 && time < 19 * 60.0f){
			video->stop(); 
			signal(this, Settings::ViewID::ENGAGE);
		}
		break;

	case ONI:
		video->stop(); 
		signal(this, Settings::ViewID::HANDBOOK); //where?
		break;
	}
}

void PlayView::onAnimateIn()
{
	ci::app::console() << "PlayView::onAnimateIn() " << std::endl;

	SoundController::getInstance().setBgVolume(0.f);

	exitingToEnd = false;
	onAnimateInComplete(); 
	//video->play(); 

	if(!mAssets->getData()->getEngageComplete())mAssets->getData()->setGamePulsed(false);
}

void PlayView::onAnimateOut()
{	
	SoundController::getInstance().setBgVolume(1.f);

	onAnimateOutComplete(); 
}
