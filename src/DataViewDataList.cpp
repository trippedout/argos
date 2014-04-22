#include "DataViewDataList.h"

using namespace ci;
using namespace ci::app;

DataViewDataList::DataViewDataList(Assets* assets) : DataViewContentBase(assets)
{
	//dont use this one, only for overrides? hacks? im a good programmer... <3 roushey. 
}

DataViewDataList::DataViewDataList(DataNodeRef dn, Assets* assets) : DataViewContentBase(assets)
{
	dataNode = dn; 
	mContentHolder	=	DisplayObjectContainerRef( new DisplayObjectContainer() );
	this->addChild(mContentHolder);
	
	this->buildContent();

	this->deselectAll();
	useSelection = true; 
	
	//moveSelection(ci::Vec2i(0,0));
}

DataViewDataList::~DataViewDataList(void)
{

}


void DataViewDataList::buildContent()
{
	
	std::vector<DataNodeDataItem*>* items = dataNode->getDataItems(); 
	std::vector<DataNodeDataItem*> dataItems = *items;
	int size = dataItems.size();

	//setup selection grid. 
	int num_of_col = 1;
	int num_of_row = size;
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	const float icon_h = 60, spacing = icon_h + 40; //h = 575 - icon_h, 

	IconRef icon; 
	InputViewRef view; 

	for( int i=0; i<size; i++){
		switch( dataItems[i]->type )
		{
		case DataNodeDataItem::DataItemType::DOCUMENT:
			view = DocRef( new DataViewDocument(dataNode, dataItems[i]->assetID, dataItems[i]->assetPath, mAssets) );
			icon = IconRef( new DataViewListIcon(mAssets, DataViewListIcon::IconType::DOCUMENT) );
			break;
		case DataNodeDataItem::DataItemType::OBJECT3D:
			view = Obj3dRef( new DataView3dObject(dataNode, dataItems[i]->assetID, mAssets) );
			icon = IconRef( new DataViewListIcon(mAssets, DataViewListIcon::IconType::OBJECT3D) );
			break;
		case DataNodeDataItem::DataItemType::MOVIE:
			view = MovieRef( new DataViewMovie(dataNode, dataItems[i]->assetID, dataItems[i]->assetPath, mAssets) );
			icon = IconRef( new DataViewListIcon(mAssets, DataViewListIcon::IconType::MOVIE) );
			break;
		case DataNodeDataItem::DataItemType::AUDIO:
			view = AudioRef( new DataViewAudio(dataNode, dataItems[i]->assetID, dataItems[i]->assetPath, mAssets) );
			icon = IconRef( new DataViewListIcon(mAssets, DataViewListIcon::IconType::AUDIO) );
			break;
		}

		mViewList.push_back(view);
		mIconList.push_back(icon);

		icon->setPosition(ci::Vec3f(0.f, 125.0f + (i*spacing), 0.f));
		addChild(icon);

		mContentMap[icon] = view;
		selectionGrid[0][i] = icon;
	}
}

void DataViewDataList::onViewOpen()
{
	show(false);
}

void DataViewDataList::onViewClose()
{
	for( unsigned int i=0; i<mIconList.size(); i++){
		timeline().apply(mIconList[i]->getPosition(), ci::Vec3f(0, 125.0f + (float)i * 100.0f, -30.0f ), 0.5f, EaseInQuint()).delay((float)i/20.0f);
		timeline().apply(mIconList[i]->getAlpha(), 0.0f, 0.5f, EaseInQuint()).delay((float)i/20.0f);
	}
	
	if(mCurrentContent) {
		timeline().apply(mCurrentContent->getAlpha(), 0.0f, 0.5f, EaseInQuint());
		timeline().apply(mCurrentContent->getPosition(), ci::Vec3f( 0, 0,-300.0f ), 0.5f, EaseInQuint());
	}
}

void DataViewDataList::show(bool snap)
{
	float m = 1.0f; 
	if(snap) m=0.01f; 

	for( unsigned int i=0; i<mIconList.size(); i++){
		mIconList[i]->setPosition(ci::Vec3f(0,  125.0f + (float)i * 100.0f, 30.0f ));
		timeline().apply(mIconList[i]->getPosition(), ci::Vec3f(0, 125.0f + (float)i * 100.0f, 0 ), 0.4f*m, EaseOutQuint()).delay((float)i/20.0f*m);
		timeline().apply(mIconList[i]->getAlpha(), 1.0f, 0.4f*m, EaseOutQuint()).delay((float)i/20.0f*m);
	}
	
	
	if(mCurrentContent) {
		mCurrentContent->setAlpha(0);	
		mCurrentContent->setPosition(ci::Vec3f( 0, 0, 40.0f ));
		timeline().apply(mCurrentContent->getAlpha(), 1.0f, 0.2f*m, EaseOutQuint());
		timeline().apply(mCurrentContent->getPosition(), ci::Vec3f( 0, 0, 0 ), 0.2f*m, EaseOutQuint());
	}
}

void DataViewDataList::hide(bool snap)
{
	float m = 1.0f; 
	if(snap) m=0; 

	for( unsigned int i=0; i<mIconList.size(); i++){
		timeline().apply(mIconList[i]->getPosition(), ci::Vec3f(0, 125.0f + (float)i * 100.0f, -30.0f ), 0.1f*m, EaseInQuint()).delay((float)i/20.0f*m);
		timeline().apply(mIconList[i]->getAlpha(), 0.0f, 0.1f*m, EaseInQuint()).delay((float)i/20.0f*m);
	}
	

	if(mCurrentContent){
		timeline().apply(mCurrentContent->getAlpha(), 0.0f, 0.2f*m, EaseInQuint());
		timeline().apply(mCurrentContent->getPosition(), ci::Vec3f( 0, 0, 40.0f ), 0.2f*m, EaseInQuint());
	}

}


void DataViewDataList::onAnimateIn()
{
	//ci::app::console()<< "DataViewDataList::onAnimateIn" << std::endl; 

	setSelection(ci::Vec2i(0,0)); 
	moveSelection(ci::Vec2i(0,0)); 
	//this->deselectAll();
	//selectedObject->selectView(true);
}

void DataViewDataList::disposeTempAssets()
{
	//ci::app::console() <<"DataViewDataList::disposeTempAssets()" <<std::endl;

	int size = mViewList.size(); 
	for( int i=0; i<size; i++){
		mViewList[i]->disposeTempAssets(); 
	}
}

void DataViewDataList::prerender()
{
	if(mCurrentContent) mCurrentContent->prerender(); 
}

void DataViewDataList::moveSelection(ci::Vec2i dir)
{
	
	InputViewRef prevContent = mCurrentContent;

	InputView::moveSelection(dir);

	//animate out current view
	if(mCurrentContent)mCurrentContent->onAnimateOut();

	//set new current and animate in
	mCurrentContent = mContentMap[selectedObject];
	mCurrentContent->onAnimateIn();
	timeline().apply(mCurrentContent->getPosition(), ci::Vec3f( 0, 0, 0 ), 0);

	int size = mViewList.size();
		
	for( int i=0; i<size; i++){
		if(mContentHolder->contains(mViewList[i])) mContentHolder->removeChild(mViewList[i]);
	}

	if(prevContent!=mCurrentContent) {
		mCurrentContent->setAlpha(0);
		timeline().apply(mCurrentContent->getAlpha(), 1.0f, 0.2f, EaseOutQuint());
	}

	mContentHolder->addChild(mCurrentContent);
}

void DataViewDataList::handleInput(INPUT_TYPE type)
{
	if(!Overlay::getInstance().getActive()){
		switch(type)
		{
		case DPAD_DOWN:
		case DOWN:
			moveSelection(ci::Vec2i(0,1));
			break;
		case DPAD_UP:
		case UP:
			moveSelection(ci::Vec2i(0,-1));
			break;
		}
	}

	if(mCurrentContent) mCurrentContent->handleInput(type);
}

#ifdef _WIN32
void DataViewDataList::handleGamepad(XINPUT_GAMEPAD gamePad)
{
	if(mCurrentContent) mCurrentContent->handleGamepad(gamePad);
}
#endif