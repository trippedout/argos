#include "HandbookDataItem.h"

using namespace ci;
using namespace ci::app;

HandbookDataItem::HandbookDataItem(HandbookBackgroundViewRef bg, DataNodeRef node, Assets* assets) : InputView(assets)
{
	background = bg; 
	dataNode = node; 
	mCurrentContent = NULL;
	object3D = NULL; 

	ci::Vec3f contentPos = ci::Vec3f(0,75.f,0);
	
	//setup buttons and their content counterparts. 
	HandbookViewOverviewRef overview = HandbookViewOverviewRef(new HandbookViewOverview(dataNode, assets));
	overview->setPosition(contentPos);
	
	overviewBtn = ButtonViewRef(new ARGOSButtonView(assets, "OVERVIEW"));
	this->addChild(overviewBtn);
	mContentMap[overviewBtn] = overview;
	views.push_back(overview); 
	buttons.push_back(overviewBtn);

	int numDataItems = dataNode->getDataItems()->size();
	int numWaypointItems = dataNode->getWaypointItems()->size(); 
	
	
	// if there are no data items, dont show any. 
	if(numDataItems > 0 ){
		
		bool isSingle3D = false; 
		//check if the node has a single data item that is a 3d object. 
		std::vector<DataNodeDataItem*>* nodes = dataNode->getDataItems();
		int numNodes = nodes->size(); 

		for( int i=0; i<numNodes; i++){
			if(nodes->at(i)->type == DataNodeDataItem::DataItemType::OBJECT3D){
				// dataNode, dataItems[i]->assetID, mAssets
				object3D = HandbookView3dObjectRef( new HandbookView3dObject(dataNode, nodes->at(i)->assetID, mAssets) );
				overview->hideImage();

				if(numNodes == 1) isSingle3D = true; 
			}
		}

		if(!isSingle3D){
			data = DVContentDisplayObjectRef(new HandbookViewDataList(dataNode, assets));
			data->setPosition(contentPos);

			dataBtn = ButtonViewRef(new ARGOSButtonView(assets, "DATA"));
			dataBtn->setPosition(ci::Vec3f(200.0f,0,0));
			this->addChild(dataBtn);
			mContentMap[dataBtn] = data;

			views.push_back(data); 
			buttons.push_back(dataBtn); 
		}
	}
	
	// if there are no waypoint items, dont show any. 
	if(numWaypointItems > 0){
		waypoint = DVContentDisplayObjectRef(new HandbookViewWaypoint(dataNode, assets));
		waypoint->setPosition(contentPos);

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
	mTitle->setPosition(ci::Vec3f(0.f,contentPos.y,0.f));
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
}

HandbookDataItem::~HandbookDataItem(void)
{

}

void HandbookDataItem::handleInput(INPUT_TYPE type)
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

void HandbookDataItem::onAnimateOut()
{
	int size = views.size(); 
	for( int i=0; i<size; i++){
		views[i]->disposeTempAssets(); 
	}
	
	float t = 0.1f;

	timeline().apply(mTitle->getPosition(), ci::Vec3f(0, 75.0f, -100.0f), t, EaseInQuint());
	timeline().apply(mTitle->getAlpha(), 0.0f, t, EaseInQuint());

	for( unsigned int i=0; i<buttons.size(); i++){
		buttons[i]->setAlpha(0);
		timeline().apply(buttons[i]->getAlpha(), 0.0f, t, EaseOutCubic()).delay((float)i/20.0f);
		timeline().apply(buttons[i]->getPosition(), ci::Vec3f(buttons[i]->getPosition()->value().x, buttons[i]->getPosition()->value().y, -100.0f), t, EaseOutCubic()).delay((float)i/20.0f);
	}

	if(mCurrentContent) mCurrentContent->onViewClose();
}

void HandbookDataItem::onAnimateIn()
{
	//ci::app::console()<< "DataInnerViewBase::onAnimateIn" << std::endl; 
	deselectAll();
	selectionPos = ci::Vec2i(0,0);
	selectedObject = selectionGrid[0][0];
	selectedObject->selectView(true);
	mCurrentContent = mContentMap[selectedObject];
	hideOffscreenViews();

	float t = 0.4f;

//animation
	mTitle->setAlpha(0);
	mTitle->setPosition(ci::Vec3f(0, 75.0f, 100.0f)); 
	timeline().apply(mTitle->getPosition(), ci::Vec3f(0, 75.0f, 0), t, EaseOutQuint());
	timeline().apply(mTitle->getAlpha(), 1.0f, t, EaseOutQuint());

	for( unsigned int i=0; i<buttons.size(); i++){
		buttons[i]->setAlpha(0);
		timeline().apply(buttons[i]->getAlpha(), 1.0f, t, EaseOutQuint()).delay((float)i/20.0f);
		buttons[i]->setPosition(ci::Vec3f(buttons[i]->getPosition()->value().x, buttons[i]->getPosition()->value().y, 100.0f));
		timeline().apply(buttons[i]->getPosition(), ci::Vec3f(buttons[i]->getPosition()->value().x, buttons[i]->getPosition()->value().y, 0), t, EaseOutQuint()).delay((float)i/20.0f);
	}

	if(mCurrentContent) mCurrentContent->onViewOpen();

	if(!object3D) background->hideObject3D();

}

void HandbookDataItem::prerender()
{
	if(mCurrentContent) mCurrentContent->prerender();
}

void HandbookDataItem::setSelection(ci::Vec2i selection)
{
	//ci::app::console() << "DataInnerViewBase::moveSelection()" << std::endl;

	InputView::setSelection(selection); //handle regular movement? 


	DVContentDisplayObjectRef old = mCurrentContent;
	mCurrentContent = mContentMap[selectedObject];
	
	if(selection.x == 0) {
		background->showObject3D();
	}else{
		background->hideObject3D();
	}

	hideOffscreenViews();

	mCurrentContent->onAnimateIn(); 
	mCurrentContent->show(false); 
}

int HandbookDataItem::getViewIndex(DVContentDisplayObjectRef view)
{
	int size = views.size(); 
	for( int i=0; i<size; i++){
		if(view == views[i]) return i;
	}

	return -1; 
}

void HandbookDataItem::hideOffscreenViews()
{
	int size = views.size(); 

	for( int i=0; i<size; i++){
		if(contains( views[i])) removeChild( views[i] );
	}

	//reset current
	if(mCurrentContent) addChild(mCurrentContent);
}

#ifdef _WIN32
void HandbookDataItem::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	if(mCurrentContent) mCurrentContent->handleGamepad(gamepad);
}
#endif