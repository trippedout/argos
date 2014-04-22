#include "LocationView.h"

using namespace ci;
using namespace ci::app;

LocationView::LocationView(Assets* assets, VideoViewRef video) : 
	InputView(assets),
	mUseTransition(false),
	mIsInTransition(false),
	mPreviousSelection(NULL),
	mMainVideo(video)
{
	//guide first!
	//createGuide("guides/location_orbital.png");
	
	//create bg views
	mOrbitalView	=	LocationViewRef( new LocationOrbitalView(assets) );
	mLocalView		=	LocationViewRef( new LocationLocalView(assets) );
	mAerialView		=	LocationViewRef( new LocationAerialView(assets) );
	
	//set default
	mCurrentView	=	mOrbitalView;

	const ci::Vec3f startingPos = ci::Vec3f(220,175,0);
	
	//create buttons
	mBtnOrbital		=	ButtonViewRef(new ARGOSButtonView(assets, "ORBITAL"));
	mBtnOrbital->setPosition(startingPos);
	mContentMap[mBtnOrbital] = mOrbitalView;
	this->addChild(mBtnOrbital);

	mBtnAerial		=	ButtonViewRef(new ARGOSButtonView(assets, "AERIAL"));
	mBtnAerial->setPosition(startingPos + ci::Vec3f(200,0,0));
	mContentMap[mBtnAerial] = mAerialView;
	this->addChild(mBtnAerial);

	mBtnLocal		=	ButtonViewRef(new ARGOSButtonView(assets, "LOCAL"));
	mBtnLocal->setPosition(startingPos + ci::Vec3f(400,0,0));
	mContentMap[mBtnLocal] = mLocalView;
	this->addChild(mBtnLocal);


	//create title and text - add real text later
	mTitle	=	TextViewRef(new TextView("CORBULO ACADEMY", assets->getFontByID("data_overview_head"), ci::ColorA(1,1,1,1)));
	mTitle->setPosition(startingPos + ci::Vec3f(-10,65,0));
	mTitle->project3D(true);
	this->addChild(mTitle);

	mSubtitle	=	TextViewRef(new TextView("OF MILITARY SCIENCE", assets->getFontByID("data_overview_subhead"), ci::ColorA(1,1,1,1)));
	mSubtitle->setPosition(startingPos + ci::Vec3f(-2,120,0));
	mSubtitle->project3D(true);
	this->addChild(mSubtitle);

	mTextBody = TvMultiDisplayObjectRef( new TextViewMultiline( assets, "Filler", ci::Vec2i(600,348)) );
	mTextBody->setPosition(startingPos + ci::Vec3f(-5,165,0));
	mTextBody->project3D(true);
	this->addChild(mTextBody);

	arrow = BugElementRef( new BugElement(assets->getTextureByID("data_item_arrow"), assets->getTextureByID("data_item_arrow_glow") ) );
	arrow->setPosition(ci::Vec3f( mTextBody->getPosition()->value().x + mTextBody->getSize()->value().x/2.0f, mTextBody->getPosition()->value().y + mTextBody->getSize()->value().y + 40.0f, 0));
	arrowLit = false;
	
	arrow->show();
	addChild(arrow);


	//setup selection
	int num_of_col = 3;
	int num_of_row = 1;
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	selectionGrid[0][0] = mBtnOrbital;
	selectionGrid[1][0] = mBtnAerial;
	selectionGrid[2][0] = mBtnLocal;

	this->deselectAll();
	useSelection = true; 
	moveSelection(ci::Vec2i(0,0));

	bgBox = ImageViewRef( new ImageView(assets->getTextureByID("bgBox")) );
	bgBox->setScale(ci::Vec3f(1920.0f/bgBox->getSize()->value().x, 1080.0f/bgBox->getSize()->value().y,0));
	bgBox->setAlpha(0);
	this->addChild(bgBox);

	mTransitions = LocationTransitionRef( new LocationTransitionView(assets) );
	this->addChild(mTransitions);

	mBack = ImageViewRef( new ImageView( assets->getTextureByID("back") ) );
	mBack->setPosition(ci::Vec3f(180,950,0));
	this->addChild(mBack);
}

LocationView::~LocationView(void)
{
	mBtnOrbital.reset();
	mBtnAerial.reset();
	mBtnLocal.reset();

	mTitle.reset();
	mSubtitle.reset();

	mTextBody.reset();

	mOrbitalView.reset();
	mAerialView.reset();
	mLocalView.reset();
	mCurrentView.reset();

	mTransitions.reset();
}

void LocationView::onAnimateIn()
{

	bgBox->setAlpha(0);

	mTransitions->onAnimateIn();

	if(mAssets->getData()->getHandbookLocation() != NULL){
		mCurrentLocation = mAssets->getData()->getHandbookLocation();
		mAssets->getData()->setHandbookLocation(NULL); 
	}else{
		mCurrentLocation = mAssets->getData()->getCurrentSceneLocation(mMainVideo->getTime());
	}

	ci::app::console() << "LocationView::onAnimateIn() currentLocation: " << mCurrentLocation->getCurrentLocationID() << std::endl;

	SoundController::getInstance().playSound( SoundController::LOC_ENTER );
	
	mUseTransition = false;

	checkCurrentLocation();
	setCurrentData();

	if( selectedObject == mBtnOrbital ) startTransition( LocationTransitionView::TransitionType::ORBITAL ); 
	else if( selectedObject == mBtnAerial ) startTransition( LocationTransitionView::TransitionType::AERIAL ); 
	else if( selectedObject == mBtnLocal ) startTransition( LocationTransitionView::TransitionType::LOCAL ); 

	//finish strong
	InputView::onAnimateInComplete();

	const ci::Vec3f startingPos = ci::Vec3f(220,175,0);

	
	
	mBtnLocal->setAlpha(0);
	mBtnAerial->setAlpha(0);
	mBtnOrbital->setAlpha(0);
	
	mBtnLocal->setPosition(startingPos + ci::Vec3f(400,0,-200));
	mBtnAerial->setPosition(startingPos + ci::Vec3f(200,0,-200));
	mBtnOrbital->setPosition(startingPos + ci::Vec3f(0,0,-200));

	timeline().apply(mBtnLocal->getAlpha(), 1.0f, 0.75f, EaseOutQuint()).delay(0.6f); 
	timeline().apply(mBtnAerial->getAlpha(), 1.0f, 0.75f, EaseOutQuint()).delay(0.55f); 
	timeline().apply(mBtnOrbital->getAlpha(), 1.0f, 0.75f, EaseOutQuint()).delay(0.5f);

	timeline().apply(mBtnLocal->getPosition(), startingPos + ci::Vec3f(400,0,0), 0.75f, EaseOutQuint()).delay(0.6f); 
	timeline().apply(mBtnAerial->getPosition(), startingPos + ci::Vec3f(200,0,0), 0.75f, EaseOutQuint()).delay(0.55f); 
	timeline().apply(mBtnOrbital->getPosition(), startingPos + ci::Vec3f(0,0,0), 0.75f, EaseOutQuint()).delay(0.5f); 


	timeline().apply(mBack->getAlpha(), 1.0f, 0.5f, EaseOutQuint()); 

	mTextBody->setAlpha(0);
	mSubtitle->setAlpha(0);
	mTitle->setAlpha(0);
	timeline().apply(mTextBody->getAlpha(), 1.0f, 0.75f, EaseOutQuint()).delay(0.5f); 
	timeline().apply(mSubtitle->getAlpha(), 1.0f, 0.75f, EaseOutQuint()).delay(0.5f); 
	timeline().apply(mTitle->getAlpha(), 1.0f, 0.75f, EaseOutQuint()).delay(0.5f); 

	mTextBody->setPosition(startingPos + ci::Vec3f(-5,165,-200));
	mSubtitle->setPosition(startingPos + ci::Vec3f(-2,120,-200));
	mTitle->setPosition(startingPos + ci::Vec3f(-10,65,-200));
	timeline().apply(mTextBody->getPosition(), startingPos + ci::Vec3f(-5, 165, 0), 1.0f, EaseOutQuint()).delay(0.5f); 
	timeline().apply(mSubtitle->getPosition(), startingPos + ci::Vec3f(-2, 120, 0), 1.0f, EaseOutQuint()).delay(0.5f); 
	timeline().apply(mTitle->getPosition(), startingPos + ci::Vec3f(-10, 65, 0), 1.0f, EaseOutQuint()).delay(0.5f); 
}

void LocationView::onAnimateOut()
{
	SoundController::getInstance().playSound(SoundController::LOC_BACK_TO_MAIN);
	//finish strong
	//InputView::onAnimateOutComplete();
	timeline().apply(bgBox->getAlpha(), 1.0f, 0.75f, EaseInQuint()).finishFn(std::bind(&LocationView::onAnimateOutComplete, this)); 

	const ci::Vec3f startingPos = ci::Vec3f(220,175,0);
	timeline().apply(mTextBody->getAlpha(), 0.0f, 0.5f, EaseInQuint()); 
	timeline().apply(mSubtitle->getAlpha(), 0.0f, 0.5f, EaseInQuint()); 
	timeline().apply(mTitle->getAlpha(), 0.0f, 0.5f, EaseInQuint()); 

	timeline().apply(mTextBody->getPosition(), startingPos + ci::Vec3f(-5,165,-150), 0.5f, EaseInQuint()); 
	timeline().apply(mSubtitle->getPosition(), startingPos + ci::Vec3f(-2,120,-150), 0.5f, EaseInQuint()); 
	timeline().apply(mTitle->getPosition(), startingPos + ci::Vec3f(-10,65,-150), 0.5f, EaseInQuint()); 

	timeline().apply(mBtnLocal->getAlpha(), 0.0f, 0.5f, EaseInQuint()).delay(0.1); 
	timeline().apply(mBtnAerial->getAlpha(), 0.0f, 0.5f, EaseInQuint()).delay(0.05);  
	timeline().apply(mBtnOrbital->getAlpha(), 0.0f, 0.5f, EaseInQuint());

	timeline().apply(mBtnLocal->getPosition(), startingPos + ci::Vec3f(400,0,-200), 0.5f, EaseInQuint()).delay(0.1); 
	timeline().apply(mBtnAerial->getPosition(), startingPos + ci::Vec3f(200,0,-200), 0.5f, EaseInQuint()).delay(0.05);
	timeline().apply(mBtnOrbital->getPosition(), startingPos + ci::Vec3f(0,0,-200), 0.5f, EaseInQuint()); 

	timeline().apply(mBack->getAlpha(), 0.0f, 0.5f, EaseInQuint()); 

	mAerialView->disposeTempAssets();
}

void LocationView::update(float dt)
{
	if(mCurrentView) mCurrentView->update(dt);

	InputView::update(dt);
}

void LocationView::prerender()
{
	if(mCurrentView) mCurrentView->prerender();
}

void LocationView::render()
{
	if(mCurrentView) mCurrentView->render();

	InputView::render();
}

void LocationView::setSelection(ci::Vec2i selection)
{
	//set previous
	if(selectedObject)
		mPreviousSelection = selectedObject;

	//continue as normal
	InputView::setSelection(selection);

	//if( mPreviousSelection == selectedObject ) return; 

	if(mUseTransition)
	{		
		if(mPreviousSelection == mBtnOrbital && selectedObject == mBtnAerial)
			startTransition(LocationTransitionView::TransitionType::ORB_AERIAL);

		else if(mPreviousSelection == mBtnAerial && selectedObject == mBtnOrbital)
			startTransition(LocationTransitionView::TransitionType::AERIAL_ORB);
		
		else if(mPreviousSelection == mBtnAerial && selectedObject == mBtnLocal)
			startTransition(LocationTransitionView::TransitionType::AERIAL_LOCAL);

		else if(mPreviousSelection == mBtnLocal && selectedObject == mBtnAerial)
			startTransition(LocationTransitionView::TransitionType::LOCAL_AERIAL);
		else
			onTransitionComplete();	
	}
	
	//first time moveSelection is called we skip transition
	mUseTransition = true; //from now on
}

void LocationView::startTransition(LocationTransitionView::TransitionType type)
{
	ci::app::console() << "LocationView::startTransition() " << type << std::endl;

	mIsInTransition = true;
	
	mCurrentView->onTransitionStart();

	mTransitions->getTransitionSignal()->connect(boost::bind(&LocationView::onTransitionListener, this, boost::arg<1>::arg()));
	mTransitions->startTransition(type);

	SoundController::getInstance().playSound(SoundController::LOC_CHANGE_PAGE);
}

void LocationView::onTransitionListener(LocationTransitionView::TransitionEvent event)
{
	ci::app::console() << "LocationView::onTransitionListener() ";

	switch(event)
	{
	case LocationTransitionView::TransitionEvent::TRANNY_SWAP:
		ci::app::console() << "TRANNY_SWAP" << std::endl;
		mCurrentView->onTransitionSwap(false); //isNext false
			setCurrentData(); // set new current	
		mCurrentView->onTransitionSwap(true); //isNext true
		break;

	case LocationTransitionView::TransitionEvent::TRANNY_END:
		ci::app::console() << "TRANNY_END" << std::endl;
		onTransitionComplete();
		break;
	}

}

void LocationView::onTransitionComplete()
{
	ci::app::console() << "LocationView::onTransitionComplete() " << std::endl;
	
	mIsInTransition = false;		

	mTransitions->getTransitionSignal()->disconnect_all_slots();
	mCurrentView->onTransitionEnd();
}

void LocationView::checkCurrentLocation()
{
	//this is kinda hacky, but itll work. 
	if(mCurrentLocation->getLocal() == NULL){
		selectionGrid[2][0] = NULL;
		removeChild(mBtnLocal);
		
		//if no local, set to aerial. 
		if(selectedObject) mPreviousSelection = selectedObject;
		InputView::setSelection(ci::Vec2i(1,0));
		//setSelection(ci::Vec2i(0,0)); 
	}else{
		selectionGrid[2][0] = mBtnLocal;
		addChild(mBtnLocal);
	}

	if(mCurrentLocation->getAerial() == NULL){
		selectionGrid[1][0] = NULL;
		removeChild(mBtnAerial);

		//if no aerial set to global 
		if(selectedObject) mPreviousSelection = selectedObject;
		InputView::setSelection(ci::Vec2i(0,0));
		//setSelection(ci::Vec2i(0,0)); 
	}else{
		selectionGrid[1][0] = mBtnAerial;
		addChild(mBtnAerial);
	}

	//set correct planet
	boost::shared_ptr<LocationOrbitalView> orbital = boost::dynamic_pointer_cast<LocationOrbitalView>(mOrbitalView);
	orbital->setPlanet(mCurrentLocation->getOrbital()->assetID);

	//clear selection on map
	boost::shared_ptr<LocationLocalView> localView = boost::dynamic_pointer_cast<LocationLocalView>(mLocalView);
	localView->setMapPosition("");

	//set correct map location and move to map if not orbital or aerial
	if(mCurrentLocation->getCurrentLocationID() == "orbital")
		setSelection(ci::Vec2i(0,0));
	else if(mCurrentLocation->getCurrentLocationID() == "aerial")
		setSelection(ci::Vec2i(1,0));
	else
	{
		//set local view
		setSelection(ci::Vec2i(2,0));
		localView->setMapPosition(mCurrentLocation->getCurrentLocationID());
	}
}

void LocationView::setCurrentData()
{
	ci::app::console() << "LocationView::setCurrentData()" << std::endl;

	//transfer to current view
	mCurrentView = mContentMap[selectedObject];

	//get proper info setup
	LocationNode::LocationInfo* info;

	if(mCurrentView == mOrbitalView)
	{
		info = mCurrentLocation->getOrbital();
	}
	else if(mCurrentView == mAerialView)
	{
		info = mCurrentLocation->getAerial();
	}
	else if(mCurrentView == mLocalView)
	{
		info = mCurrentLocation->getLocal();
	}
	
	if(info)
	{
		mTitle->setText( info->head );
		mSubtitle->setText( info->subhead );
		mTextBody->setText( info->body );

		if(mTextBody->scrollable()){
			arrow->setPosition(ci::Vec3f( mTextBody->getPosition()->value().x + mTextBody->getSize()->value().x/2.0f, mTextBody->getPosition()->value().y + mTextBody->getSize()->value().y + 40.0f, 0));
			arrow->show();
			addChild(arrow);
		}else{
			arrow->hide();
			removeChild(arrow);
		}
	}
}

void LocationView::handleInput(INPUT_TYPE type)
{
	//transition? DONT DO SHIT
	if(mIsInTransition) return;

	//pass through movements
	InputView::handleInput(type);

	switch(type)
	{
	case BACK:
		signal(this, Settings::ViewID::BACK);
		return;

	//voice stuffs
	case PRESSED_X:
	case HANDBOOK:
		signal(this, Settings::ViewID::HANDBOOK);
		break;
	
	case ORBITAL:
		setSelection(ci::Vec2i(0,0));
		return;
	case AERIAL:
		setSelection(ci::Vec2i(1,0));
		return;
	case LOCAL:
		setSelection(ci::Vec2i(2,0));
		return;
            
        default:
            break;
	}

	/*if(mCurrentView == mLocalView)*/ mCurrentView->handleInput(type);
}

#ifdef _WIN32
void LocationView::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	if(mCurrentView == mLocalView) mCurrentView->handleGamepad(gamepad);

	if(!mIsInTransition){
		if(mTextBody->scrollable())
		{
			int max = 32000; //ish? 
			ci::Vec2f left = ci::Vec2f((float)gamepad.sThumbLX/(float)max, gamepad.sThumbLY/(float)max ); 
		
			// dpad overrides the left stick. 
			int dpadUp = (int)(gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
			int dpadDown = (int)(gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
	
			if(  dpadUp > 0 ) {
				left.y = -0.75f; 
			} else if( dpadDown > 0 ) {
				left.y = 0.75f; 
			}

			if(!arrowLit && left.y > 0.25f || left.y < -0.25f){
				arrowLit = true; 
				arrow->glow();
			}
			mTextBody->handleGamepad(gamepad);
		}else{
			if(mTextBody->scrollable() && arrowLit){
				arrowLit = false;
				arrow->show(); 
			}
		}
	}
}
#endif
