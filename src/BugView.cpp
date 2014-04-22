#include "BugView.h"

using namespace ci;
using namespace ci::app;

BugView::BugView(Assets* assets, VideoViewRef vid) : View(assets)
{
	checkDataTimer = checkDataTimerMax = 1.0f;

	dataController = assets->getData();
	video = vid; 

	previousScene = dataController->getScenes()->at(0);

	bug = boost::shared_ptr< Bug >( new Bug(assets) );
	bug->setPosition(ci::Vec3f(1730, 930, 0)); 

	addChild(bug); 
}

void BugView::resetOniPulses()
{
	//make oni symbol markers pulse again. 
	std::vector< boost::shared_ptr<DataNode> >* nodes = dataController->getDataNodes();
	int size = nodes->size(); 
	for( int i =0; i<size; i++){
		if(nodes->at(i)->isOni()){
			if(nodes->at(i)->isOniLocked()){
				nodes->at(i)->setOniPulsed(false);
			}
		}
	}

	dataController->setGamePulsed(false);
}

void BugView::update(float dt)
{
	View::update(dt);

	if( currentView == Settings::ViewID::PLAY || currentView == Settings::ViewID::DATA ){
		checkDataTimer-=dt; 

		if( checkDataTimer < 0 ){
			checkDataTimer = checkDataTimerMax;

			float time = video->getVideo()->getCurrentTime();

			bool pulseDown = false; 
			bool pulseRight = false; 
			bool pulseLeft = false; 
			bool pulseUp = false; 
			
			bool oniUp = false; 
			bool oniDown = false; 
			bool oniLeft = false; 
			bool oniRight = false; 

			SceneRef currentScene = dataController->getCurrentScene( time );

			if(currentScene != previousScene){
				previousScene = currentScene;
				resetOniPulses();
				ci::app::console()<< "BugView::update() scene has changed, resetting ONI pulses" <<std::endl;
			}

			//data & oni
			std::vector< boost::shared_ptr<DataNode> >* nodes = currentScene->getCurrentDataNodes( time );
			int size = nodes->size(); 
			for( int i =0; i<size; i++){
				if(nodes->at(i)->isLocked()){
					pulseDown = true; 
					nodes->at(i)->setLocked(false);
				}

				if(nodes->at(i)->isOni() && !nodes->at(i)->isOniPulsed()){
					if(nodes->at(i)->isOniLocked()){
						oniDown = true; 
						pulseDown = true; 
						nodes->at(i)->setOniPulsed(true);
					}
				}
			}

			//locations
			boost::shared_ptr<LocationNode> location = currentScene->getCurrentLocation( time );
			if( location->isLocked() ){
				location->setLocked(false);
				pulseRight = true; 
			}

			std::vector< boost::shared_ptr<BeaconData> > beacons = *currentScene->getBeacons();
			size = beacons.size(); 
			for( int i=0; i<size; i++){
				if(beacons[i]->isNew){
					pulseUp = true; 
					beacons[i]->isNew = false;
				}
			}
			
			//engage
			if(time > 18*60.0f + 2 && time < 19 * 60.0f && !dataController->getGamePulsed() && !dataController->getEngageComplete() ){
				dataController->setGamePulsed(true);
				pulseLeft = true;
				oniLeft = true; 
			}

			if(pulseDown) bug->pulseDown(oniDown); 
			if(pulseRight) bug->pulseRight(oniRight); 
			if(pulseLeft) bug->pulseLeft(oniLeft); 
			if(pulseUp) bug->pulseUp(oniUp); 

			if( (pulseDown || pulseRight || pulseLeft || pulseUp) && !bug->isVoiceActive() ){
				SoundController::getInstance().playSound( SoundController::BUG_COMBINED );
			}
		}
	}

}

void BugView::changeView(Settings::ViewID id)
{
	currentView = id;  

	if(id == Settings::ViewID::DATA){
		bug->selectDown(); 

	}else if(id == Settings::ViewID::LOCATION){
		bug->selectRight(); 

	}else if(id == Settings::ViewID::ENGAGE){
		bug->selectLeft(); 

	}else if(id == Settings::ViewID::BEACON){
		bug->selectUp(); 

	}else if(id == Settings::ViewID::START
		|| id == Settings::ViewID::PREFS
		|| id == Settings::ViewID::HOST
		|| id == Settings::ViewID::END){
		bug->hide(); 

	}else if(id == Settings::ViewID::PAUSE
		|| id == Settings::ViewID::HANDBOOK){
		bug->progress(); 

	}else{
		bug->idle();
	}

	if(id == Settings::ViewID::PLAY){
		timeline().apply( bug->getScale(), ci::Vec3f(0.85f,0.85f,0.85f), 0.5f, EaseOutQuint() );
	}else {
		timeline().apply( bug->getScale(), ci::Vec3f(1.0f,1.0f,1.0f), 0.5f, EaseOutQuint() );
	}
}

void BugView::voiceOn()
{
	SoundController::getInstance().playSound( SoundController::UI_NOTIFY );
	bug->voiceOn();
	timeline().apply( bug->getScale(), ci::Vec3f(1.0f,1.0f,1.0f), 0.5f, EaseOutQuint() );
}

void BugView::voiceOff()
{
	SoundController::getInstance().playSound( SoundController::UI_NOTIFY );
	bug->voiceOff();

	if(currentView == Settings::ViewID::PLAY){
		timeline().apply( bug->getScale(), ci::Vec3f(0.85f,0.85f,0.85f), 0.5f, EaseOutQuint() );
	}else {
		timeline().apply( bug->getScale(), ci::Vec3f(1.0f,1.0f,1.0f), 0.5f, EaseOutQuint() );
	}
}

void BugView::idle()
{
	bug->idle();
}

void BugView::data()
{
	bug->idle();
	bug->selectDown(); 
}

void BugView::progress()
{
	bug->progress();
}

BugView::~BugView(void)
{
}
