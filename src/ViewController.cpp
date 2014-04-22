#include "ViewController.h"

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

ViewController::ViewController(Assets* ass, InputController* ic) :
	assets(ass),
	inputController(ic),
	firstRun(true),
	showBackground(true),
	backgroundController(NULL),
	video(NULL),
	bug(NULL),
	mCurrentView(NULL),
	current( Settings::ViewID::START ),
	viewChangeDelay(0),
	inputDelay(0)
{
	//time our creation
	ci::app::console() << "ViewController creation!" << std::endl;
	double startTime = getElapsedSeconds();


	//setup inputController signal
	inputController->registerSignal(boost::bind(&ViewController::handleInput, this, boost::arg<1>::arg()));
	
	//overlay
	Overlay::getInstance().initView(assets);
	
	//background
	if(showBackground) backgroundController = new BackgroundController(assets); 

	//prepare scenes for the video. 
	std::vector<boost::shared_ptr<Scene>> dataScenes = *assets->getData()->getScenes();
	std::vector<float> videoScenes; 
	for( int i=0; i< (int)dataScenes.size(); i++){
		videoScenes.push_back( dataScenes[i]->getStart() ); 
	}

	//video
	video = VideoViewRef( new VideoObject( assets->getVideoByID("main_video"), videoScenes ) );
	video->setPosition(ci::Vec3f(0,0,0)); 
	video->setScale(ci::Vec3f(2.0f, 2.0f, 1.0f)); 

	videoSnapshot = VideoSnapshotRef( new VideoSnapshot(video, assets) ); 
	videoSnapshot->setScale(video->getScale()->value()); 
	videoSnapshot->setPosition(video->getPosition()->value()); 

	//bug view is always present, and rendered over everything. 
	bug = new BugView(assets, video); 
	
	//setup all your views
	addView(Settings::ViewID::PAUSE, InputViewRef(new PauseView(assets, video, videoSnapshot))); 
	addView(Settings::ViewID::PLAY, InputViewRef(new PlayView(assets, video))); 
	addView(Settings::ViewID::PREFS, InputViewRef(new PrefsView(assets))); 
	addView(Settings::ViewID::HOST, InputViewRef(new HostView(assets))); 
	addView(Settings::ViewID::HANDBOOK, InputViewRef(new HandbookView(bug, assets))); 
	addView(Settings::ViewID::DATA, InputViewRef(new DataView(assets, video, videoSnapshot, backgroundController))); 
	addView(Settings::ViewID::LOCATION, InputViewRef(new LocationView(assets, video))); 
	//addView(Settings::ViewID::ANALYZE, new AnalyzeView(assets)); 
	addView(Settings::ViewID::ENGAGE, InputViewRef(new EngageView(videoSnapshot, assets))); 
	addView(Settings::ViewID::END, InputViewRef(new EndView(assets, video))); 
	addView(Settings::ViewID::BEACON, InputViewRef(new BeaconView(assets, video)));
	addView(Settings::ViewID::START, InputViewRef(new StartView(assets))); 

	//get started on start - derp
	mCurrentView = views[current];

	ci::app::console() << "SHIT DONE: " << getElapsedSeconds() - startTime << std::endl;
}

ViewController::~ViewController(void)
{
	delete inputController;
	delete backgroundController; 
}

void ViewController::update(float dt)
{
	if(inputDelay > 0) inputDelay-=dt; 
	if(viewChangeDelay >= 0) viewChangeDelay-=dt; 

	// update background if video is not playing
	if(mCurrentView != views[Settings::ViewID::PLAY] && showBackground ) backgroundController->update(dt);
	
	mCurrentView->update(dt);
    
#ifdef _WIN32
	mCurrentView->handleGamepad(inputController->getGamepad());
#endif
	
	Overlay::getInstance().getView()->update(dt);

	bug->update(dt);

	/*
	::BULLSHIT HACK::
	Apparently Cinders timeline() implementation doesn't work until after the initial update() and
	draw() loop is fired. Plus once it starts some objects are a bit squeamish so we're waiting not just
	one frame, but 60 to start the intro and play the video.
	*/

	if( firstRun && getElapsedFrames() > 60 )
	{
		boost::dynamic_pointer_cast<StartView>(views[Settings::ViewID::START])->startIntro();
		firstRun = false;
	}
	
}

void ViewController::render(int screenWidth, int screenHeight)
{
	if(!Overlay::getInstance().getFullScreen()){

		//do any pre-rendering. FBO's etc.
		glPushMatrix();
			backgroundController->prerender();
			mCurrentView->prerender(); 
		glPopMatrix();


		float fov = 60.0f; 
		float camZ = (screenHeight*0.5f) / tan(fov * 0.5f * (float)0.0174532925) * -1.0f;
		
		//setup main camera
		CameraPersp cam( screenWidth, screenHeight, fov );
		cam.setPerspective( 60.0f, getWindowAspectRatio(), 1.0f, 10000.0f );
		camPos = ci::Vec3f( (float)screenWidth/2.0f, (float)screenHeight/2.0f, camZ );
		camTarget = ci::Vec3f( (float)screenWidth/2.0f, (float)screenHeight/2.0f, 0 );
		camUp = ci::Vec3f( 0, -1.0f, 0 );
		cam.lookAt( camPos, camTarget, camUp);
		
		//render shit transformed by camera
		glPushMatrix();
		{
			gl::setMatrices( cam );
			gl::setViewport( getWindowBounds() );

			// render background if video is not playing
			if(mCurrentView != views[Settings::ViewID::PLAY] && showBackground ) backgroundController->render();
			
			//hack! reset the matrices so that background doesnt fuck shit up
			gl::setMatrices( cam );
			gl::setViewport( getWindowBounds() );

			//render current view
			 mCurrentView->render(); 
		}
		glPopMatrix();
	}

	//render anything else, NOT effected by camera. 
	Overlay::getInstance().getView()->render();
	bug->render(); 
}

void ViewController::addView(Settings::ViewID id, InputViewRef view)
{
	view->getSignal()->connect(boost::bind(&ViewController::onViewChangeSelection, this, boost::arg<1>::arg(), boost::arg<2>::arg()));
	views.insert( std::pair<Settings::ViewID, InputViewRef>(id, view) );
}

void ViewController::onViewChangeSelection(InputView* view, Settings::ViewID id)
{
	if(id == Settings::ViewID::BACK){
		back();
	}else{
		if(mCurrentView == views[id]) return;
		changeView(id);
	}
}

void ViewController::back()
{
	int size = backStack.size(); 
	if( size == 0 ) return; 
	
	Settings::ViewID prevID = backStack.back();

	ci::app::console() << "BACK: " << prevID << " current:" << current << " back stack: "; 
	for( int i=0; i<size; i++){
		ci::app::console() << backStack[i]<< " , ";
	}
	ci::app::console() << std::endl; 

	//remove 
	
	backStack.pop_back(); 
	
	changeView(prevID, true);
}

void ViewController::changeView(Settings::ViewID id, bool fromBack)
{
	//handbook -> engage specific code
	if(current == Settings::ViewID::HANDBOOK && id == Settings::ViewID::ENGAGE)
	{
		back();
		return;
	}

	if(mCurrentView == views[id] || !views[id]  ) return; 
	
	viewChangeDelay = 0.25f;

	ci::app::console() << "change view to: " << id << " current:" << current <<std::endl; 
	
	//is fromBack? dont add
	if(!fromBack) backStack.push_back(current);

	//set current as the next id
	current = id; 

	//queue up the next view
	mNextView = views[id];

	//animate the current view out and listen for its complete
	mCurrentView->addViewStateSignal(boost::bind(&ViewController::handleViewAnimationState, this, boost::arg<1>::arg(), boost::arg<2>::arg()));
	mCurrentView->onAnimateOut(id);
}

void ViewController::handleViewAnimationState(View* view, View::AnimState state)
{
	int size = backStack.size();

	switch(state)
	{
	case View::AnimState::ANIMATE_OUT_COMPLETE:
		ci::app::console() << view << " animateOut complete!" << std::endl;
		
		//get rid of the animateOut signal
		view->removeViewStateSignal();
		
		//set the next view to current so it can render correctly
		mCurrentView = mNextView;

		//setup a listener so we know when its finished animating in
		mNextView->addViewStateSignal(boost::bind(&ViewController::handleViewAnimationState, this, boost::arg<1>::arg(), boost::arg<2>::arg()));
		
		//update necessary overlays/background
		bug->changeView(current); 
		if(showBackground) backgroundController->changeView(current);

		if(size <= 0){
			mNextView->onAnimateIn( Settings::ViewID::START );
		}else{
			mNextView->onAnimateIn( backStack.back() );
		}
		break;

	case View::AnimState::ANIMATE_IN_COMPLETE:
		ci::app::console() << "animateIn complete!" << std::endl;
		//remove the listener that we added in animateOutComplete
		mCurrentView->removeViewStateSignal();
		break;
            
        default:
            break;
	}
}

void ViewController::handleInput(INPUT_TYPE type)
{
	if( viewChangeDelay > 0 ) return; 

	//check immediately for any of the evergreen commands
	switch(type)
	{	
	case UNLOCK_ALL_DATA:
		assets->getData()->unlockAllContent();
		if(mCurrentView != views[Settings::ViewID::PLAY]){
			video->stop();
			video->reset(); 
		}
		if(mCurrentView != views[Settings::ViewID::START]) changeView(Settings::ViewID::START);
		return;

	case LOCK_ALL_DATA:
		assets->getData()->lockAllContent();
		if(mCurrentView != views[Settings::ViewID::PLAY]){
			video->stop();
			video->reset(); 
		}
		if(mCurrentView != views[Settings::ViewID::START]) changeView(Settings::ViewID::START);
		
		return;

	//case DPAD_UP://hack to turn it on and off without saying XBOX
	case VOICE_ACTIVATE:
		bug->voiceOn(); 
		break;
		
	//case DPAD_DOWN://hack to turn it on and off without saying XBOX
	case VOICE_DEACTIVATE:
		bug->voiceOff(); 
		break;
            
        default:
            break;
	
	}

	//if none of everygreen, pass handling through
	mCurrentView->handleInput(type);
	
}

void ViewController::dPad(ci::Vec2i dir)
{
	//TODO: fix this. 
	//changeView(Settings::ViewID::DATA); 
	
	/*
	//TODO: this is pretty damned ghetto. 
	if(current == Settings::ViewID::PLAY || current == Settings::ViewID::DATA || current == Settings::ViewID::ANALYZE || current == Settings::ViewID::LOCATION ){
		// you should only be able to access engage from the play view? 
		
		
		changeView(Settings::ViewID::DATA); 

		if(dir.x == 0 && dir.y > 0 && current == Settings::ViewID::PLAY ) changeView(Settings::ViewID::ENGAGE); 
		if(dir.x == 0 && dir.y < 0) changeView(Settings::ViewID::DATA); 
		if(dir.y == 0 && dir.x < 0) changeView(Settings::ViewID::ANALYZE); 
		if(dir.y == 0 && dir.x > 0) changeView(Settings::ViewID::LOCATION); 
	}
	*/
}

void ViewController::selectOld()
{
	mCurrentView->selectOld();
}

void ViewController::showHandbook()
{
	changeView(Settings::ViewID::HANDBOOK); 
}

void ViewController::showBeacon()
{
	//do something with this? 
}

void ViewController::showStart()
{
	if(mCurrentView == views[Settings::ViewID::START]) return; 

	video->reset(); 
	video->stop();
	changeView(Settings::ViewID::START); 
}

void ViewController::showPause()
{
	if(mCurrentView != views[Settings::ViewID::PLAY]) return; 

	changeView(Settings::ViewID::PAUSE); 
}

InputViewRef ViewController::getCurrent()
{
	return views[current]; 
}