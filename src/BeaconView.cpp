#include "BeaconView.h"

using namespace ci;
using namespace ci::app;

BeaconView::BeaconView(Assets* assets, VideoViewRef video) :
	InputView(assets),
	mIsShowing(false),
	mVideo(video)
{
	//select back
	mSelectBack = ImageViewRef( new ImageView( assets->getTextureByID("select_back") ) );
	mSelectBack->setPosition(ci::Vec3f(180,950,0));
	this->addChild(mSelectBack);

	//create tabs
	const int leftMargin = 180;

	mReceivedTab	=	ButtonViewRef( new BeaconTabView(assets, "RECEIVED") );
	mReceivedTab->setPosition(ci::Vec3f(leftMargin,170,0));
	this->addChild(mReceivedTab);

	mCreateTab		=	ButtonViewRef( new BeaconTabView(assets, "+BEACON") );
	mCreateTab->setPosition(ci::Vec3f(leftMargin + 200,170,0));
	this->addChild(mCreateTab);

	//create sections
	const ci::Vec3f vp = ci::Vec3f(leftMargin,250,0);

	mBeaconReceivedView = BeaconSubViewRef( new BeaconReceivedView(assets) );
	mBeaconReceivedView->setPosition(vp);
	mContentMap[mReceivedTab] = mBeaconReceivedView;
	this->addChild(mBeaconReceivedView);
	
	mBeaconAddNewView = BeaconSubViewRef( new BeaconAddNewView(assets) );
	mBeaconAddNewView->setPosition(vp);
	mBeaconAddNewView->setAlpha(0.f);
	mContentMap[mCreateTab] = mBeaconAddNewView;
	this->addChild(mBeaconAddNewView);


	//set selection
	int num_of_col = 2;
	int num_of_row = 1;
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	selectionGrid[0][0] = mReceivedTab;
	selectionGrid[1][0] = mCreateTab;

	this->deselectAll();
	useSelection = true; 
	moveSelection(ci::Vec2i(0,0));
}

BeaconView::~BeaconView(void)
{
	mReceivedTab.reset();
	mCreateTab.reset();

	mBeaconReceivedView.reset();
	mBeaconAddNewView.reset();
	mCurrentView.reset();
}


void BeaconView::onAnimateIn()
{
	//update received beacons
	boost::shared_ptr<BeaconReceivedView> ptr = boost::dynamic_pointer_cast<BeaconReceivedView>(mBeaconReceivedView);
	ptr->updateBeacons( 
		mAssets->getData()->getCurrentScene( mVideo->getTime() )->getBeacons()
		);

	timeline().apply(mVideo->getAlpha(), .5f, 1.f);

	onAnimateInComplete();
}

void BeaconView::onAnimateOut()
{	
	timeline().apply(mVideo->getAlpha(), 1.f, .5f);

	onAnimateOutComplete();
}

void BeaconView::render()
{
	//if(mIsComingFromVideo) renderVideo();
	mVideo->render();
	InputView::render();
}

void BeaconView::renderVideo()
{
	//for now
	mVideo->render();
}

void BeaconView::moveSelection(ci::Vec2i dir)
{
	InputView::moveSelection(dir);

	//get rid of old
	if(mCurrentView) mCurrentView->setAlpha(0.f);

	//set new
	mCurrentView = mContentMap[selectedObject];
	mCurrentView->setAlpha(1.f);
}

void BeaconView::handleInput(INPUT_TYPE type)
{
	if(mCurrentView == mBeaconReceivedView)
		mBeaconReceivedView->handleInput(type);

	if(type == SELECT) SoundController::getInstance().playSound(SoundController::SELECT);

	if(this->useSelection)
	{
		InputView::handleInput(type);
		
		switch(type)
		{
		case BACK:
			signal(this, Settings::ViewID::BACK);
			return;

		case DOWN:
		case SELECT:
			if(selectedObject == mCreateTab) 
				setActive(false);
			break;
		}
	}
	else
		mCurrentView->handleInput(type);
	
}

void BeaconView::playMovementSound(ci::Vec2i selection)
{
	InputView::playMovementSound(selection);

	ButtonViewRef prev, next;

	//if( (prevSelectedObject == mReceivedTab
}

void BeaconView::onViewComplete(BeaconSubView::CompletionType type)
{
	setActive(true);

	//if its done, GET THE FUCK OUT
	if(type == BeaconSubView::CompletionType::COMPLETE)
		signal(this, Settings::ViewID::BACK);
}

void BeaconView::setActive(bool active)
{
	this->useSelection = active;
	mCurrentView->setActive(!active);

	//setup a listener to wait for the views to respond
	if(active)
		mCurrentView->getCompletionSignal()->disconnect_all_slots();
	else
		mCurrentView->getCompletionSignal()->connect(boost::bind(&BeaconView::onViewComplete, this, boost::arg<1>::arg()));
}

bool BeaconView::getIsShowing()
{
	return mIsShowing;
}