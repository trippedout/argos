#include "HandbookViewDataList.h"

using namespace ci;
using namespace ci::app;

HandbookViewDataList::HandbookViewDataList(DataNodeRef dn, Assets* assets):
	DataViewDataList(assets)
{
	dataNode = dn; 
	mContentHolder	=	DisplayObjectContainerRef( new DisplayObjectContainer() );
	this->addChild(mContentHolder);
	
	this->buildContent();

	this->deselectAll();
	useSelection = true; 
}

void HandbookViewDataList::onViewClose()
{
	float t = 0.1f;

	for( unsigned int i=0; i<mIconList.size(); i++){
		timeline().apply(mIconList[i]->getPosition(), ci::Vec3f(0, 125.0f + (float)i * 100.0f, -100.0f ), t, EaseOutQuint()).delay((float)i/20.0f);
		timeline().apply(mIconList[i]->getAlpha(), 0.0f, t, EaseOutQuint()).delay((float)i/20.0f);
	}
	
	if(mCurrentContent) {
		timeline().apply(mCurrentContent->getAlpha(), 0.0f, t, EaseOutQuint());
		timeline().apply(mCurrentContent->getPosition(), ci::Vec3f( 0, -50.0f, 0 ), t, EaseOutQuint());
	}
}

void HandbookViewDataList::show(bool snap)
{
	float t = 0.4f;
	float m = 1.0f; 
	if(snap) m=0.01f; 

	for( unsigned int i=0; i<mIconList.size(); i++){
		mIconList[i]->setPosition(ci::Vec3f(0,  125.0f + (float)i * 100.0f, 30.0f ));
		timeline().apply(mIconList[i]->getPosition(), ci::Vec3f(0, 125.0f + (float)i * 100.0f, 0 ), t*m, EaseOutQuint()).delay((float)i/20.0f*m);
		timeline().apply(mIconList[i]->getAlpha(), 1.0f, t*m, EaseOutQuint()).delay((float)i/20.0f*m);
	}
	
	if(mCurrentContent) {
		mCurrentContent->setAlpha(0);	
		mCurrentContent->setPosition(ci::Vec3f( 0, 50.0f, 0 ));
		timeline().apply(mCurrentContent->getAlpha(), 1.0f, t*m*2.0f, EaseOutQuint());
		timeline().apply(mCurrentContent->getPosition(), ci::Vec3f( 0, 0, 0 ), t*m, EaseOutQuint());
	}
}

void HandbookViewDataList::buildContent()
{
	std::vector<DataNodeDataItem*>* items = dataNode->getDataItems(); 
	std::vector<DataNodeDataItem*> dataItems = *items;
	int size = dataItems.size();

	int numModels = 0 ; 
	for( int i=0; i<size; i++){
		if(dataItems[i]->type == DataNodeDataItem::DataItemType::OBJECT3D){
			numModels++;
		}
	}

	//ci::app::console() << "HandbookViewDataList::buildContent ID: " << dataNode->imageID << " \n\t dataItems: " << size << "\n\t 3d models: "<< numModels << std::endl;

	//setup selection grid. 
	int num_of_col = 1;
	int num_of_row = size - numModels;

	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	const float icon_h = 60, spacing = icon_h + 40; //h = 575 - icon_h, 

	IconRef icon; 
	InputViewRef view; 

	int itemIndex = 0; 
	for( int i=0; i<size; i++){
		bool isModel = false; 

		switch( dataItems[i]->type )
		{
		case DataNodeDataItem::DataItemType::DOCUMENT:
			view = DocRef( new HandbookViewDocument(dataNode, dataItems[i]->assetID, dataItems[i]->assetPath, mAssets) );
			icon = IconRef( new DataViewListIcon(mAssets, DataViewListIcon::IconType::DOCUMENT, dataItems[i]->iconLabel ) );
			break;
		
		//dont show 3d objects... beacause they'll already be there. 
		case DataNodeDataItem::DataItemType::OBJECT3D:
			isModel = true; 
			break;
		
		case DataNodeDataItem::DataItemType::MOVIE:
			view = MovieRef( new HandbookViewMovie(dataNode, dataItems[i]->assetID, dataItems[i]->assetPath, mAssets) );
			icon = IconRef( new DataViewListIcon(mAssets, DataViewListIcon::IconType::MOVIE, dataItems[i]->iconLabel ) );
			break;

		case DataNodeDataItem::DataItemType::AUDIO:
			view = AudioRef( new DataViewAudio(dataNode, dataItems[i]->assetID, dataItems[i]->assetPath, mAssets) );
			icon = IconRef( new DataViewListIcon(mAssets, DataViewListIcon::IconType::AUDIO, dataItems[i]->iconLabel ) );
			break;
		}

		if(!isModel){
			mViewList.push_back(view);
			mIconList.push_back(icon);

			icon->setPosition(ci::Vec3f(0.f, 125.0f + (i*spacing), 0.f));
			addChild(icon);

			mContentMap[icon] = view;
			selectionGrid[0][itemIndex] = icon;
			itemIndex++;
		}
	}
}

HandbookViewDataList::~HandbookViewDataList(void)
{
}
