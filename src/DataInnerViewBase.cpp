
#include "DataInnerViewBase.h"

using namespace ci;
using namespace ci::app;

DataInnerViewBase::DataInnerViewBase(boost::shared_ptr<DataNode> dn,  Assets* assets) : InputView(assets)
{
	dataNode = dn;
	//initial position just for ref
	//this->setPosition(Vec3i(365,175,0));
	//initialize shits
	
	lightBeam = ImageViewRef( new ImageView( assets->getTextureByID("data_lightPulse") ) );
	lightBeam->setCenter(ci::Vec3f(0.5f, 0, 0)); 
	lightBeam->setScale(ci::Vec3f(5.0f, 1.5f, 0));
	lightBeam->setAdditiveBlending(true);
	lightBeam->setAlpha(0);
	addChild(lightBeam);

	mFirstRun = true;
	mCurrentContent = NULL;

	float contentY = 75.f;
	mLeftVec = ci::Vec3f(-(float)getWindowWidth(),contentY,0);
	mCenterVec = ci::Vec3f(0,contentY,0);
	mRightVec = ci::Vec3f((float)getWindowWidth(),contentY,0);
	

	//setup buttons and their content counterparts. 
	DVContentDisplayObjectRef overview	=	DVContentDisplayObjectRef(new DataViewOverview(dataNode, assets));
	overview->setPosition(mCenterVec);
	
	overviewBtn = ButtonViewRef(new ARGOSButtonView(assets, "OVERVIEW"));
	this->addChild(overviewBtn);
	mContentMap[overviewBtn] = overview;
	views.push_back(overview); 
	buttons.push_back(overviewBtn);


	int numDataItems = dataNode->getDataItems()->size();
	int numWaypointItems = dataNode->getWaypointItems()->size(); 
	
	// if there are no data items, dont show any. 
	if(numDataItems > 0){
		data = DVContentDisplayObjectRef(new DataViewDataList(dataNode, assets));
		data->setPosition(mCenterVec);

		dataBtn = ButtonViewRef(new ARGOSButtonView(assets, "DATA"));
		dataBtn->setPosition(ci::Vec3f(200.0f,0,0));
		this->addChild(dataBtn);
		mContentMap[dataBtn] = data;

		views.push_back(data); 
		buttons.push_back(dataBtn); 
	}
	
	// if there are no waypoint items, dont show any. 
	if(numWaypointItems > 0){
		waypoint = DVContentDisplayObjectRef(new DataViewWaypoint(dataNode, assets));
		waypoint->setPosition(mCenterVec);

		float x = 200.0f; 
		if(data) x = 400.0f; 
		waypointBtn = ButtonViewRef(new ARGOSButtonView(assets, "WAYPOINT"));
		waypointBtn->setPosition(ci::Vec3f(x,0,0));
		this->addChild(waypointBtn);
		mContentMap[waypointBtn] = waypoint;

		views.push_back(waypoint); 
		buttons.push_back(waypointBtn); 
	}
	

	//setup title view
	mTitle = DVTitleDisplayObjectRef(new DataViewTitle(dataNode, assets));
	mTitle->setPosition(ci::Vec3f(0.f,contentY,0.f));
	this->addChild(mTitle);

	//setup selection grid
	// TODO: make this more elegant. 
	int num_of_col = buttons.size();
	int num_of_row = 1;
	int i; 

	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	for( i=0; i<num_of_col; i++){
		selectionGrid[i][0] = buttons[i];
	}

	this->deselectAll();
	useSelection = true; 
	//moveSelection(ci::Vec2i(0,0));

	//styling
	/*guide = new ImageView( new ci::gl::Texture( loadImage( loadAsset( "guides/data.png"))));

	createParams();
	params->addParam("data", &mTitle->getPosition()->value() );*/
}

DataInnerViewBase::~DataInnerViewBase(void)
{

}

void DataInnerViewBase::show(bool snap)
{
	//animate in shit
	setAlpha(1.0f);
	mTitle->setAlpha(1.0f);

	float m = 1.0f; 
	if(snap) m=0.01f; 

	//35.0f
	float x = -getPosition()->value().x - mTitle->getSize()->value().x/2.0f;
	mTitle->setPosition(ci::Vec3f(x, 75.0f, 0)); 
	//timeline().apply(mTitle->getPosition(), ci::Vec3f(x, 75.0f, 0), 0.2f*m, EaseOutQuint());
	timeline().apply(mTitle->getPosition(), ci::Vec3f(0, 75.0f, 0), 0.4f*m, EaseOutQuint());
	mTitle->slideHack(0.4f*m); 

	for( unsigned int i=0; i<buttons.size(); i++){
		buttons[i]->setPosition(ci::Vec3f(buttons[i]->getPosition()->value().x, buttons[i]->getPosition()->value().y, 0));
		buttons[i]->setAlpha(0);
		timeline().apply(buttons[i]->getAlpha(), 1.0f, 0.1f*m, EaseOutQuint()).delay((float)i/30.0f*m);
	}

	for(unsigned int i=0; i<views.size(); i++){
		views[i]->show(snap);
	}
}

void DataInnerViewBase::hide(bool snap)
{
	//animate out shit
	
	float m = 1.0f; 
	if(snap) m=0.01f; 

	float x = -getPosition()->value().x - mTitle->getSize()->value().x/2.0f;

	//35.0f
	mTitle->setPosition(ci::Vec3f(0, 75.0f, 0)); 
	timeline().apply(mTitle->getPosition(), ci::Vec3f(x, 75.0f, 0), 0.25f*m, EaseOutQuint());
	timeline().apply(mTitle->getAlpha(), 0.0f, 0.15f*m, EaseOutQuint()).delay(0.1f*m);
	//timeline().appendTo(mTitle->getPosition(), ci::Vec3f(x, 35.0f, 0), 0.2f*m, EaseOutQuint());

	for( unsigned int i=0; i<buttons.size(); i++){
		buttons[i]->setPosition(ci::Vec3f(buttons[i]->getPosition()->value().x, buttons[i]->getPosition()->value().y, 0));
		timeline().apply(buttons[i]->getAlpha(), 0.0f, 0.1f*m, EaseOutQuint()).delay((float)i/30.0f*m);
	}

	if(mCurrentContent){
		mCurrentContent->hide(snap);
	}
	//alpha out buttons
}


void DataInnerViewBase::closeView()
{
	//ci::app::console() <<"DataInnerViewBase::closeView()" <<std::endl;

	int size = views.size(); 
	for( int i=0; i<size; i++){
		views[i]->disposeTempAssets(); 
	}

	//ci::app::console() << std::endl;
}

void DataInnerViewBase::onAnimateIn()
{
	mTitle->setAlpha(0);
	mTitle->setPosition(ci::Vec3f(0, 75.0f, -400.0f)); 
	timeline().apply(mTitle->getPosition(), ci::Vec3f(0, 75.0f, 0), 1.5f, EaseOutQuint());
	timeline().apply(mTitle->getAlpha(), 1.0f, 1.5f, EaseOutQuint());

	reset();

	for( unsigned int i=0; i<buttons.size(); i++){
		buttons[i]->setAlpha(0);
		timeline().apply(buttons[i]->getAlpha(), 1.0f, 1.0f, EaseOutQuint()).delay((float)i/20.0f);
		buttons[i]->setPosition(ci::Vec3f(buttons[i]->getPosition()->value().x, buttons[i]->getPosition()->value().y, -400.0f));
		timeline().apply(buttons[i]->getPosition(), ci::Vec3f(buttons[i]->getPosition()->value().x, buttons[i]->getPosition()->value().y, 0), 1.0f, EaseOutQuint()).delay((float)i/20.0f);
	}
	
	if(mCurrentContent) mCurrentContent->onViewOpen();
}

void DataInnerViewBase::onAnimateOut()
{
	timeline().apply(mTitle->getPosition(), ci::Vec3f(0, 75.0f, 100.0f), 0.5f, EaseInQuint());
	timeline().apply(mTitle->getAlpha(), 0.0f, 0.5f, EaseInQuint());

	for( unsigned int i=0; i<buttons.size(); i++){
		buttons[i]->setAlpha(0);
		timeline().apply(buttons[i]->getAlpha(), 0.0f, 0.5f, EaseOutQuint()).delay((float)i/20.0f);
		timeline().apply(buttons[i]->getPosition(), ci::Vec3f(buttons[i]->getPosition()->value().x, buttons[i]->getPosition()->value().y, -400.0f), 0.4f, EaseOutQuint()).delay((float)i/20.0f);
	}

	if(mCurrentContent) mCurrentContent->onViewClose();
}

void DataInnerViewBase::prerender()
{
	if(mCurrentContent) mCurrentContent->prerender();
}

void DataInnerViewBase::reset()
{
	deselectAll();
	selectionPos = ci::Vec2i(0,0);
	selectedObject = selectionGrid[0][0];
	selectedObject->selectView(true);
	mCurrentContent = mContentMap[selectedObject];
	mCurrentContent->setPosition(mCenterVec);
	hideOffscreenViews();
}

void DataInnerViewBase::setSelection(ci::Vec2i selection)
{
	//ci::app::console() << "DataInnerViewBase::moveSelection()" << std::endl;

	InputView::setSelection(selection); //handle regular movement? 


	DVContentDisplayObjectRef old = mCurrentContent;
	mCurrentContent = mContentMap[selectedObject];
	
	//ci::app::console() << "DataInnerViewBase::moveSelection() current: " << mCurrentContent->getPosition()->value() << std::endl;
	
	float delay = 0.5f;
	

	if(old){
		int oldIndex = getViewIndex(old); 
		int newIndex = getViewIndex(mCurrentContent); 
		
		if( newIndex > oldIndex  ){
			addChild(mCurrentContent);
			mCurrentContent->setPosition(mRightVec);
			timeline().apply(old->getPosition(), mLeftVec, delay, EaseOutCubic());

			lightBeam->setPosition( mRightVec + ci::Vec3f(640.0f, -256.0f, 0) );
			timeline().apply(lightBeam->getPosition(), mLeftVec + ci::Vec3f(600.0f, -256.0f, 0), delay*1.5f, EaseOutCubic());

		}else if( newIndex < oldIndex  ){
			addChild(mCurrentContent);
			mCurrentContent->setPosition(mLeftVec);
			timeline().apply(old->getPosition(), mRightVec, delay, EaseOutCubic());

			lightBeam->setPosition( mLeftVec + ci::Vec3f(640.0f, -256.0f, 0) );
			timeline().apply(lightBeam->getPosition(), mRightVec + ci::Vec3f(600.0f, -256.0f, 0), delay*1.5f, EaseOutCubic());
		}else{
			// current is old, dont do anything. 
			mCurrentContent->setPosition( mCenterVec );
			hideOffscreenViews();
			return; 
		}

		lightBeam->setAlpha(0.0f); 
		timeline().apply(lightBeam->getAlpha(), 0.5f, delay*0.25f*1.5f, EaseInQuint());
		timeline().appendTo(lightBeam->getAlpha(), 0.0f, delay*0.25f*1.5f, EaseOutQuint()).delay(delay*0.5f*1.5f);

		timeline().apply(mCurrentContent->getPosition(), mCenterVec, delay, EaseOutCubic())
			.finishFn(std::bind(&DataInnerViewBase::hideOffscreenViews,this));
	}else{
		//there is no old content, just set the position. 
		mCurrentContent->setPosition(mCenterVec);
		hideOffscreenViews();
	}

	mCurrentContent->onAnimateIn(); 

	mFirstRun = false;
}

int DataInnerViewBase::getViewIndex(DVContentDisplayObjectRef view)
{
	int size = views.size(); 
	for( int i=0; i<size; i++){
		if(view == views[i]) return i;
	}

	return -1; 
}

void DataInnerViewBase::hideOffscreenViews()
{
	int size = views.size(); 

	for( int i=0; i<size; i++){
		if(contains( views[i])) removeChild( views[i] );
	}

	//reset current
	if(mCurrentContent) addChild(mCurrentContent);
}

void DataInnerViewBase::handleInput(INPUT_TYPE type)
{
	if(!Overlay::getInstance().getActive()){
		switch(type)
		{
		case DPAD_LEFT:
		case LEFT:
			moveSelection(ci::Vec2i(-1,0));
			break;
			
		case DPAD_RIGHT:
		case RIGHT:
			moveSelection(ci::Vec2i(1,0));
			break;

			//voice
		case OVERVIEW:
			setSelection(ci::Vec2i(0,0));
			break;

		case DATA:
			if(dataBtn)
				setSelection(ci::Vec2i(1,0));
			break;
			
		case WAYPOINT:
			if(waypointBtn)
			{
				if(dataBtn)
					setSelection(ci::Vec2i(2,0));
				else
					setSelection(ci::Vec2i(1,0));
			}
                
            default:
                break;
		}
	}

	if(mCurrentContent) mCurrentContent->handleInput(type);
}

#ifdef _WIN32
void DataInnerViewBase::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	if(mCurrentContent) mCurrentContent->handleGamepad(gamepad);
}
#endif