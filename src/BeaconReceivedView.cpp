#include "BeaconReceivedView.h"

using namespace ci;
using namespace ci::app;

BeaconReceivedView::BeaconReceivedView(Assets* assets) : 
	BeaconSubView(assets),
	mBeacons(),
	mBeaconListItems(new std::vector<BeaconReceivedListItemRef>),
	mNextToOpen(NULL)
{
	this->useSelection = true;
}

BeaconReceivedView::~BeaconReceivedView(void)
{
	delete mBeacons;
	delete mBeaconListItems;
}

void BeaconReceivedView::updateBeacons(BeaconVec beacons)
{
	ci::app::console() << "BeaconReceivedView::updateBeacons() numBeacons: " << beacons->size() << std::endl;

	this->setSelection(ci::Vec2i(-1,-1));

	//set ref
	mBeacons = beacons;
	unsigned int i;

	//clear list
	for(i = 0; i < mBeaconListItems->size(); ++i)
	{
		this->removeChild(mBeaconListItems->at(i));
	}
	mBeaconListItems->clear();


	//setup display list for selection
	
	int num_of_col = 1;
	unsigned int num_of_row = mBeacons->size();

	selectionGrid = std::vector< std::vector< ButtonViewRef > >();
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>((int)num_of_row , NULL) );

	//add new shit
	for(i = 0; i < num_of_row; ++i)
	{
		BeaconReceivedListItemRef listItem = BeaconReceivedListItemRef( new BeaconReceivedListItem(mAssets, mBeacons->at(i)));
		listItem->setPosition(ci::Vec3f(0.f, i * listItem->getSize()->value().y, 0.f)); 
		listItem->setListPosition(i);
		this->addChild(listItem);
		
		mBeaconListItems->push_back(listItem);

		selectionGrid[0][i] = listItem;
	}

	this->setSelection(ci::Vec2i(0,0));
}

void BeaconReceivedView::setSelection(ci::Vec2i selection)
{
	InputView::setSelection(selection);

	this->handleInput(SELECT);
}

void BeaconReceivedView::handleInput(INPUT_TYPE type)
{
	//handle normal movement
	InputView::handleInput(type);

	switch(type)
	{
	case BACK:
		//mCompletionSignal();
		break;
	case SELECT:
		mNextToOpen = boost::dynamic_pointer_cast<BeaconReceivedListItem>(selectedObject);

		ci::app::console() << "BeaconReceivedView::handleInput() SELECT: " << selectedObject << " and ptr: " << mNextToOpen << std::endl;
		
		if( !closeOpenItems() ) onCloseItem();
		break;
	}
}

bool BeaconReceivedView::closeOpenItems()
{
	ci::app::console() << "BeaconReceivedView::closeOpenItems()" << std::endl;
	//look for open items
	for(unsigned int i = 0; i < mBeaconListItems->size(); ++i)
	{
		if(mBeaconListItems->at(i)->getIsOpen())
		{
			ci::app::console() << "\tItem Open! - closing" << std::endl;
			closeItem( mBeaconListItems->at(i) );
			return true;
		}
	}

	//return false if none are open
	return false;
}

void BeaconReceivedView::closeItem(BeaconReceivedListItemRef listItem)
{
	ci::app::console() << "BeaconReceivedView::closeItem() open:" << listItem->getIsOpen() << std::endl;
	listItem->close();

	if( listItem->getListPosition() + 1 < mBeaconListItems->size() )
	{
		for(unsigned int i = listItem->getListPosition() + 1; i < mBeaconListItems->size(); ++i)
		{
			BeaconReceivedListItemRef item = mBeaconListItems->at(i);
			
			if( i == mBeaconListItems->size() - 1 )
				timeline().apply(item->getPosition(), ci::Vec3f(0.f, i * listItem->getSize()->value().y, 0.f), .35f, EaseInOutCubic())
					.finishFn(boost::bind(&BeaconReceivedView::onCloseItem, this));
			else
				timeline().apply(item->getPosition(), ci::Vec3f(0.f, i * listItem->getSize()->value().y, 0.f), .35f, EaseInOutCubic());
		}
	}
	else
	{
		//the item closing is literally at the end of the list
		onCloseItem();
	}
}

void BeaconReceivedView::onCloseItem()
{
	ci::app::console() << "BeaconReceivedView::onCloseItem() nextToOpen: " << mNextToOpen << std::endl;

	if(mNextToOpen)
	{
		openItem(mNextToOpen);
	}
}

void BeaconReceivedView::openItem(BeaconReceivedListItemRef listItem)
{
	ci::app::console() << "BeaconReceivedView::openItem() " << listItem << std::endl;

	listItem->open();

	if( listItem->getListPosition() + 1 < mBeaconListItems->size() )
	{
		for(unsigned int i = listItem->getListPosition() + 1; i < mBeaconListItems->size(); ++i)
		{
			BeaconReceivedListItemRef item = mBeaconListItems->at(i);

			if( i == mBeaconListItems->size() - 1 )
				timeline().apply(item->getPosition(), ci::Vec3f(0.f, i * item->getSize()->value().y + 50.f, 0.f), .35f, EaseInOutCubic())
					.finishFn(boost::bind(&BeaconReceivedView::onOpenItem, this));
			else
				timeline().apply(item->getPosition(), ci::Vec3f(0.f, i * item->getSize()->value().y + 50.f, 0.f), .35f, EaseInOutCubic());
		}
	}
}

void BeaconReceivedView::onOpenItem()
{
	ci::app::console() << "BeaconReceivedView::onOpenItem()" << std::endl;

	mNextToOpen = NULL;
}

void BeaconReceivedView::setActive(bool active)
{
	
}