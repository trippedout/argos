#include "HandbookView.h"

using namespace ci;
using namespace ci::app;

HandbookView::HandbookView(BugView* bug, Assets* assets) : 
	InputView(assets),
	mBackgroundView(HandbookBackgroundViewRef(new HandbookBackgroundView(assets))),
	inData(false),
	inputDelay(0),
	bug(bug),
	mHandbookToLocation(ScrubVideoRef( new ScrubVideoObject(assets->getVideoByID("loc_hb_orbital") )))
{

	mHandbookToLocation->setScale(ci::Vec3f(2.0f,2.0f,0)); 
	mHandbookToLocation->setAdditiveBlending(true);

	mTitle = TextViewRef(new TextView("HANDBOOK", mAssets->getFontByID(Assets::FileID::DEFAULT_FONT), ci::ColorA(1,1,1,1)));
	mTitle->setPosition( ci::Vec3f(178,149,0) );

	mDataView = HandbookDataViewRef(new HandbookDataView( mBackgroundView, assets ));
	mNavigationView = HandbookNavigationViewRef(new HandbookNavigationView( mBackgroundView, assets));

	//navigation view
	mNavigationView->getDataSelectedSignal()->connect(boost::bind(&HandbookView::onDataSelected, this, boost::arg<1>::arg()));
	mNavigationView->getLocationSelectedSignal()->connect(boost::bind(&HandbookView::onLocationSelected, this, boost::arg<1>::arg()));

	mNavigationView->setPosition( ci::Vec3f(178, 149 + 60, 0) ); 

	mAddBack = ImageViewRef( new ImageView( assets->getTextureByID("select_back") ) );
	mAddBack->setPosition(ci::Vec3f(180,950,0));
	
	this->addChild(mBackgroundView);
	this->addChild(mNavigationView);
	this->addChild(mDataView);
	this->addChild(mAddBack);
	this->addChild(mTitle);
}

HandbookView::~HandbookView(void)
{
	mNavigationView.reset();
	mDataView.reset();
	mBackgroundView.reset();
	mAddBack.reset();
}

void HandbookView::onDataSelected(DataNodeRef dataNode)
{
	ci::app::console() << "HandbookView::onDataSelected() " << dataNode->title << std::endl;
	
	inData = true; 

	//bug->idle(); 
	bug->data(); 


	mDataView->setDataNode(dataNode);

	mNavigationView->onAnimateOut();
	mDataView->onAnimateIn(); 

	inputDelay = 0.5f;
}

void HandbookView::onLocationSelected(LocationNodeRef locationNode)
{
	if(inputDelay > 0) return; 

	ci::app::console() << "HandbookView::onLocationSelected() " << locationNode->getOrbital()->head << std::endl;
	
	mAssets->getData()->setHandbookLocation(locationNode); 

	signal(this, Settings::ViewID::LOCATION);
	
	inputDelay = 1.0f;
}

void HandbookView::exitData()
{
	inData = false; 

	bug->progress(); 

	mNavigationView->onAnimateIn();
	mDataView->onAnimateOut(); 

	inputDelay = 0.5f;
}

void HandbookView::handleInput(INPUT_TYPE type)
{
	if(inputDelay > 0) return; 

	//dont do anything if the overlay is active (video is playing) 
	if( !Overlay::getInstance().getActive() ){
		switch(type)
		{
		case BACK:
			if(inData){
				exitData();
			}else{
				signal(this, Settings::ViewID::BACK);
			}
			break;
		}
	}
	
	if(inData){
		mDataView->handleInput(type);
	}else{
		mNavigationView->handleInput(type);
	}
}

#ifdef _WIN32
void HandbookView::handleGamepad( XINPUT_GAMEPAD gamepad )
{
	if(inData){
		mDataView->handleGamepad(gamepad);
	}
	
	mBackgroundView->handleGamepad(gamepad);
}
#endif

void HandbookView::update(float dt)
{
	if(inputDelay > 0) inputDelay-=dt; 

	InputView::update(dt);
}

void HandbookView::render()
{
	InputView::render();
}

void HandbookView::prerender()
{
	if(inData) mDataView->prerender();
	mBackgroundView->prerender();
}

void HandbookView::onAnimateIn(Settings::ViewID from)
{
	removeChild(mHandbookToLocation); 

	inputDelay = 0.5f;

	mAssets->getData()->setHandbookLocation(NULL);  

	if( from == Settings::ViewID::ENGAGE ) {
		mNavigationView->showOniTab(); 
	}else if(from != Settings::ViewID::LOCATION ){
		//mNavigationView->showFirstTab();
	}
	
	mTitle->setAlpha(0); 
	timeline().apply(mTitle->getAlpha(), 1.0f, 0.25f);

	mNavigationView->onAnimateIn();
	mBackgroundView->onAnimateIn();
}

void HandbookView::onAnimateIn()
{
	onAnimateIn(Settings::ViewID::START);
}


void HandbookView::onAnimateOut(Settings::ViewID to)
{
	//mHandbookToLocation
	float transitionTime = 0.5f; 

	if( to == Settings::ViewID::LOCATION){
		transitionTime = mHandbookToLocation->getDuration() * 0.5f; 

		mHandbookToLocation->setTime(0);
		mHandbookToLocation->play(); 
		addChild(mHandbookToLocation); 
	}

	inputDelay = transitionTime;

	timeline().apply(mTitle->getAlpha(), 0.0f, 0.25f); 

	mNavigationView->onAnimateOut();
	mBackgroundView->onAnimateOut();
	
	timeline().apply(getAlpha(), 1.0f, transitionTime).finishFn(std::bind(&HandbookView::onAnimateOutComplete,this)); 
}

void HandbookView::onAnimateOutComplete()
{
	mHandbookToLocation->stop();
	removeChild(mHandbookToLocation); 

	InputView::onAnimateOutComplete();
}

void HandbookView::onAnimateOut()
{
	onAnimateOut(Settings::ViewID::START);
}


