#include "HandbookLocationList.h"


HandbookLocationList::HandbookLocationList(Assets* assets) : 
	HandbookList(assets), 
	mLocations( assets->getData()->getLocationNodes() )
{
	mNumCategories = mLocations->size();
	ci::app::console() << "HandbookLocationList mNumCategories :" << mNumCategories << std::endl;

	buildListItems();
	updateData();

	this->useSelection = true;

	updateSelectionList();
}


void HandbookLocationList::buildListItems()
{
	ci::app::console() << "HandbookLocationList buildListItems mNumCategories :" << mNumCategories << std::endl;

	for(unsigned int i = 0; i < mNumCategories; ++i)
	{
		LocationNodeRef location = mLocations->at(i);

		//create list items
		HandbookListItemRef item = HandbookListItemRef( new HandbookListItem(mAssets, location->getOrbital()->head ) );
		item->setPosition( ci::Vec3f(0, i * item->getSize()->value().y, 0 ));
		item->setListIndex((int)i);
		item->setAlpha(0.f);
		mListItems->push_back(item);
		this->addChild(item);

		//create subitems
		SubItemList subItems = new std::vector<HandbookListSubItemRef>;
		float containerOffset = i * item->getSize()->value().y + item->getSize()->value().y + SUB_LIST_PADDING;
		
		HandbookListSubItemRef subitem;

		//orbital, aerial, local
		if( location->getOrbital() != NULL ){
			subitem = HandbookListSubItemRef( new HandbookListSubItem(mAssets, "Orbital", "orbital", location ));
			subItems->push_back(subitem);
		}
		if( location->getAerial() != NULL ){
			subitem = HandbookListSubItemRef( new HandbookListSubItem(mAssets, "Aerial", "aerial",location ));
			subItems->push_back(subitem);
		}
		if( location->getLocal() != NULL ){
			subitem = HandbookListSubItemRef( new HandbookListSubItem(mAssets, "Local", "local", location ));
			subItems->push_back(subitem);
		}

		for(unsigned  j=0; j<subItems->size(); j++){
			subItems->at(j)->setPosition(ci::Vec3f(100.f, j * subitem->getSize()->value().y + containerOffset, 0.f));
			this->addChild(subItems->at(j));
		}

		//set map for selection
		mListItemSubMap[item] = subItems;
	}
}

//iterate through all the list items and see if they are locked or not
void HandbookLocationList::updateData()
{
	ci::app::console() << "HandbookLocationList buildListItems mNumCategories :" << mNumCategories << std::endl;

	for(unsigned int i = 0; i < mNumCategories; ++i)
	{
		LocationNodeRef location = mLocations->at(i);
		HandbookListItemRef item = mListItems->at(i);

		//create subitems
		SubItemList subItems = mListItemSubMap[item];

		item->setLocked( location->isLocked() );

		for(unsigned int j = 0; j < subItems->size(); ++j)
		{
			HandbookListSubItemRef subitem = subItems->at(j);
			subitem->setLocked( location->isLocked() ); 
		}
	}
}


void HandbookLocationList::handleInput(INPUT_TYPE type)
{
	//dont do shit while animating lists
	 
	if(mIsAnimating) return;
	
	//handle basic movement
	InputView::handleInput(type);

	//check for clicks
	switch(type)
	{
	case SELECT:
		HandbookListItemRef listItem = boost::dynamic_pointer_cast<HandbookListItem>(selectedObject);
		if( listItem && !listItem->getLocked() ) { toggleListItem(listItem); return; }

		HandbookListSubItemRef subItem = boost::dynamic_pointer_cast<HandbookListSubItem>(selectedObject);
		if( subItem && !subItem->getLocked()){
			subItem->getLocationNode()->setCurrentLocationID( subItem->getLocationID() ); 
			mLocationItemClickedSignal(subItem->getLocationNode());
		}
		break;
	}
}

HandbookLocationList::~HandbookLocationList(void)
{
}
