#include "DataViewMovie.h"

using namespace ci::app;

DataViewMovie::DataViewMovie(boost::shared_ptr<DataNode> dn, std::string assetID, std::string assetP,  Assets* assets) : InputView(assets)
{
	assetPath = assetP;
	dataNode = dn;
	thumbnail = ImageViewRef( new ImageView(assets->getTextureByID(assetID) ) );
	thumbnail->setPosition(ci::Vec3f(100.0f,80.0f,0.0f)); 
	thumbnail->setAdditiveBlending(true);

	thumbBG = ImageViewRef( new ImageView(assets->getTextureByID("hb_video")) ); 
	thumbBG->setPosition(ci::Vec3f(100.0f,80.0f,0.0f)); 
	thumbBG->setAlpha(0.6f);

	rate  = 0; 
	seekSpeed = 100.0f; 
	frameSeekDelay = 0;
	inputDelay = 0;
	fastforwarding = false; 
	rewinding = false;

	fullScreen = false;
	playing = false;
	video = NULL;
	
	addChild(thumbBG); 
	addChild(thumbnail); 
}

DataViewMovie::~DataViewMovie(void)
{
	if(video){
		video->stop();
		video.reset();
	}
}

void DataViewMovie::disposeTempAssets()
{
	//ci::app::console() << "DataViewMovie deleteing video!" << std::endl;
	//video->deleteVideo(); 
	if(video){
		video->stop(); 
		video.reset();
		video = NULL;
	}
}

void DataViewMovie::onAnimateOut()
{
	disposeTempAssets();
}

void DataViewMovie::update(float dt)
{
	if(inputDelay >= 0) inputDelay-=dt;
	if(frameSeekDelay >= 0) frameSeekDelay-=dt; 

	InputView::update(dt);


	if(video && fullScreen && (fastforwarding || rewinding) ){
		if(video->getVideo()->isPlaying()) video->stop(); 

		float time = video->getVideo()->getCurrentTime();
		float m = rewinding ? -1 : 1;

		if(frameSeekDelay <0){
			frameSeekDelay = 1/12.0f;
			video->getVideo()->seekToTime( time + dt * m + seekSpeed * dt * m * rate ); 
		}
	}

	if(video && fullScreen && playing ){

		float t = video->getVideo()->getCurrentTime();
		float d = video->getVideo()->getDuration();


		if( t > d*0.99f){
			exitFullScreen();
		}
	}
}

void DataViewMovie::exitFullScreen()
{
	if(fullScreen){
		fullScreen = false;
		playing = false;
		Overlay::getInstance().setActive(false);
		Overlay::getInstance().setFullScreen(false);
		
		if(video){
			Overlay::getInstance().getView()->removeChild(video);
			video->stop();
			video->reset();
		}
		
		SoundController::getInstance().setBgVolume(1.f);
	}
}

#ifdef _WIN32
void DataViewMovie::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	if( !video || !fullScreen )return;

	int left = (int)gamepad.bLeftTrigger;
	int right = (int)gamepad.bRightTrigger;
	//ci::app::console() << "DataViewMovie::handleGamepad()" << std::endl;

	if( left >= 100 && !fastforwarding ){
		if(inputDelay <= 0){
			ci::app::console() << "DataViewMovie::handleGamepad() rewinding" << std::endl;
			rewinding = true; 
			inputDelay = 0.5f;
		}
	}else if( left < 100 && rewinding ){
		rewinding = false; 
		inputDelay = 0.5f;
		if(!video->getVideo()->isPlaying()) video->play(); 
	}
	
	if( right >= 100 && !rewinding){
		if(inputDelay <= 0){
			ci::app::console() << "DataViewMovie::handleGamepad() fastforwarding" << std::endl;
			fastforwarding = true; 
			inputDelay = 0.5f;
		}
	}else if( right < 100 && fastforwarding){
		fastforwarding = false; 
		inputDelay = 0.5f;
		if(!video->getVideo()->isPlaying()) video->play(); 
	}

	if(rewinding) rate = (left-100.0f)/155.0f;
	if(fastforwarding) rate = (right-100.0f)/155.0f;

	if( !fastforwarding && !rewinding ) rate = 0;
}
#endif

void DataViewMovie::handleInput(INPUT_TYPE type)
{
	switch(type)
	{
	case NEXT:
		if(video)video->nextScene();
		break;

	case PREVIOUS:
		if(video)video->prevScene();
		break;

	case BACK:
		//close video if its open. 
		exitFullScreen();
		break;

	case SELECT:
		//close video if its open. 
		if(!fullScreen){
			SoundController::getInstance().setBgVolume(0.f);
			fullScreen = true; 
			playing = true; 
			Overlay::getInstance().setActive(true);
			Overlay::getInstance().setFullScreen(true);
			
			if(!video) video = VideoViewRef( new VideoObject( new ci::qtime::MovieGl( loadAsset(assetPath) ) ) );

			if(video){
				Overlay::getInstance().getView()->addChild(video);
				video->reset();
				video->play();
				video->setScale(ci::Vec3f(2.0f,2.0f,2.0f)); 
				video->setPosition(ci::Vec3f(0,0,0));
			}
			


		}else{
			if(!fastforwarding && !rewinding){
				if(playing){
					playing = false;
					if(video) video->stop();
				}else{
					playing = true;
					if(video) video->play();
				}
			}
		}
		break;

	default:
            break;
	}
}