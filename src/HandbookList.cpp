#include "HandbookList.h"

using namespace ci;
using namespace ci::app;

HandbookList::HandbookList(Assets* assets, CategoryList categories) : 
	InputView(assets),
	mCategories(categories),
	mNumCategories(categories->size()),
	mListItems(new std::vector<HandbookListItemRef>),
	mActiveListItem(NULL),
	mIsAnimating(false)
{
	buildListItems();
	updateData();

	this->useSelection = true;

	updateSelectionList();
}

HandbookList::HandbookList(Assets* assets) : 
	InputView(assets),
	mListItems(new std::vector<HandbookListItemRef>),
	mActiveListItem(NULL),
	mIsAnimating(false)
{
	//dont use this one.. this is a hack to let handbookLocationList work...
}


HandbookList::~HandbookList(void)
{
	mActiveListItem.reset();
	
	mListItems->clear();
	mListItemSubMap.clear();
}

void HandbookList::moveSelection(ci::Vec2i dir)
{
	InputView::moveSelection(dir);

	HandbookListSubItemRef subItem = boost::dynamic_pointer_cast<HandbookListSubItem>(selectedObject);
	if( subItem )
	{
		//ci::app::console() << "HandbookList::moveSelection CONSTELLATION: "<< subItem->getConstellation() << std::endl;
		mSubItemSelectedSignal( subItem->getConstellation() );
	}
}

void HandbookList::buildListItems()
{
	for(unsigned int i = 0; i < mNumCategories; ++i)
	{
		HBCategoryRef category = mCategories->at(i);

		//create list items
		HandbookListItemRef item = HandbookListItemRef( new HandbookListItem(mAssets, category->getTitle()) );
		item->setPosition( ci::Vec3f(0, i * item->getSize()->value().y, 0 ));
		item->setListIndex((int)i);
		item->setAlpha(0.f);
		mListItems->push_back(item);
		this->addChild(item);


		//create subitems
		std::vector<DataNodeRef>* nodes = category->getNodes();
		SubItemList subItems = new std::vector<HandbookListSubItemRef>;
		float containerOffset = i * item->getSize()->value().y + item->getSize()->value().y + SUB_LIST_PADDING;
		
		for(unsigned int j = 0; j < nodes->size(); ++j)
		{
			HandbookListSubItemRef subitem = HandbookListSubItemRef( new HandbookListSubItem(mAssets, nodes->at(j)->title, nodes->at(j) ));
			subitem->setPosition(ci::Vec3f(100.f, j * subitem->getSize()->value().y + containerOffset, 0.f));
			this->addChild(subitem);
			subItems->push_back(subitem);
		}

		//set map for selection
		mListItemSubMap[item] = subItems;
	}
}

void HandbookList::onAnimateIn()
{
	updateData();

	bool listsAreOpen=false;

	for(unsigned int i=0; i<mListItems->size(); i++){
		HandbookListItemRef listItem = boost::dynamic_pointer_cast<HandbookListItem>(mListItems->at(i));
		if( listItem && listItem->getIsShowingSublist() ){
			//list is already open, no need to open a new one. 
			listsAreOpen=true;
			
			SubItemList subItemList = mListItemSubMap[listItem];
			unsigned int numSubItems = subItemList->size();

			//animate out individual sub items
			for(unsigned int i = 0; i < numSubItems; ++i)
			{
				subItemList->at(i)->onAnimateIn(0.1f + i/20.0f);
			}
		}
	}

	if(!listsAreOpen){
		//no lists currently open, open first unlocked listItem
		for(unsigned int i=0; i<mListItems->size(); i++){
			HandbookListItemRef listItem = boost::dynamic_pointer_cast<HandbookListItem>(mListItems->at(i));
			if( listItem && !listItem->getLocked()){
				setSelection(ci::Vec2i(0,i));
				openSubList(listItem);
				break;
			}
		}
	}
	
	for(unsigned int i = 0; i < mNumCategories; ++i){
		HandbookListItemRef listItem = mListItems->at(i);
		listItem->setAlpha(0.0f);
		timeline().apply(listItem->getAlpha(), 1.0f, 0.4f, EaseInQuint() );
	}

	onAnimateInComplete();
}

void HandbookList::onAnimateOut()
{
	mSubItemClickedSignal.disconnect_all_slots();

	for(unsigned int i = 0; i < mNumCategories; ++i){
		HandbookListItemRef listItem = mListItems->at(i);
		timeline().apply(listItem->getAlpha(), 0.0f, 0.0f, EaseInQuint() ); 
		
		if(listItem->getIsShowingSublist()){
			SubItemList subItemList = mListItemSubMap[listItem];
			unsigned int numSubItems = subItemList->size();

			//animate out individual sub items
			for(unsigned int i = 0; i < numSubItems; ++i)
			{
				subItemList->at(i)->onAnimateOut();
			}
		}
	}
}


//iterate through all the list items and see if they are locked or not
void HandbookList::updateData()
{
	for(unsigned int i = 0; i < mNumCategories; ++i)
	{
		HBCategoryRef category = mCategories->at(i);
		HandbookListItemRef item = mListItems->at(i);

		//create subitems
		std::vector<DataNodeRef>* nodes = category->getNodes();
		SubItemList subItems = mListItemSubMap[item];

		bool itemLocked = true; 

		for(unsigned int j = 0; j < subItems->size(); ++j)
		{
			DataNodeRef node = nodes->at(j);
			HandbookListSubItemRef subitem = subItems->at(j);
			
			if( !node->isLocked() ) itemLocked = false;
			subitem->setLocked( node->isLocked() ); 
		}

		item->setLocked(itemLocked);
	}
}

void HandbookList::updateSelectionList()
{
	//ci::app::console() << "HandbookList::updateSelectionList()" << std::endl;
	
	//check if item is selected already
	ci::Vec2i last = ci::Vec2i(-1,-1);

	if(selectionPos != ci::Vec2i(-1,-1))
		last = selectionPos;

	unsigned int i = 0;
	int num_of_col = 1;
	int num_of_row = (int)mListItems->size();

	//check if anything is active
	for(i = 0; i < num_of_row; ++i)
	{
		if(mListItems->at(i)->getIsShowingSublist())
		{
			//ci::app::console() << "\t listItem showing sublist! adding sublist to overall size!" << std::endl;
			//ci::app::console() << "\t listItem setting selected position to: "<< i << std::endl;
			last = ci::Vec2i(0,i); 
			num_of_row += mListItemSubMap[mListItems->at(i)]->size();
			break;
		}
	}

	//ci::app::console() << "\ttotal num_of_row: " << num_of_row << std::endl;
	
	selectionGrid = std::vector< std::vector< ButtonViewRef > >();
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	//set actual grid
	unsigned int offset = 0;

	for(i = 0; i < num_of_row; ++i)
	{
		//ci::app::console() << "\tlooping through items : " << i << " offset: " << offset << std::endl;
		HandbookListItemRef item = mListItems->at(i - offset);

		selectionGrid[0][i] = item;

		if(item->getIsShowingSublist())
		{
			//ci::app::console() << "\tadd on shit below----" << std::endl;

			SubItemList subItems = mListItemSubMap[item];
			offset = subItems->size();

			for(unsigned int j = 0; j < offset; ++j)
			{
				i++;
			//	ci::app::console() << "\t\tadding sublist item: " << i << std::endl;
				selectionGrid[0][i] = subItems->at(j);	
			}
		}
	}
	
	if(last != ci::Vec2i(-1,-1))
		setSelection(last);
	else
		setSelection(ci::Vec2i(0,0));
}

void HandbookList::handleInput(INPUT_TYPE type)
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
		if( subItem && !subItem->getLocked() )
		{
			mSubItemClickedSignal(subItem->getDataNode());
			SoundController::getInstance().playSound(SoundController::HB_SELECT_ITEM);
		}
		break;
            
            
	}
}


void HandbookList::toggleListItem(HandbookListItemRef listItem)
{
	//ci::app::console() << "HandbookList::toggleListItem()" << std::endl;

	//check if listitem is active now, and just close itself
	if(mActiveListItem == listItem)
	{
		closeSubList(listItem);
		return;
	}

	//its not currently active, so setup for animation
	mNextActiveListItem = listItem;

	//if its not the currently selected object, check for other
	//open list items and close them
	bool listsAreOpen = closeOpenLists();

	//if no lists are open we need to manually call onListCloseComplete() - typically only our first run
	//or if a user selects a currently open list and its toggled
	if(!listsAreOpen) onListCloseComplete();
}

bool HandbookList::closeOpenLists()
{
	//ci::app::console() << "HandbookList::closeOpenLists()" << std::endl;

	bool listsAreOpen = false;

	for(unsigned int i = 0; i < mListItems->size(); ++i)
	{
		HandbookListItemRef li = mListItems->at(i);

		if(li->getIsShowingSublist())
		{
			closeSubList(li);
			listsAreOpen = true;
			break;
		}
	}

	return listsAreOpen;
}

void HandbookList::closeSubList(HandbookListItemRef listItem)
{
	//ci::app::console() << "HandbookList::closeSubList()" << std::endl;

	SubItemList subItemList = mListItemSubMap[listItem];
	unsigned int numSubItems = subItemList->size();


	//animate out individual sub items
	for(unsigned int i = 0; i < numSubItems; ++i)
	{
		subItemList->at(i)->onAnimateOut();
	}


	//once complete, animate back the buttons
	//as long as its not the last list item in the list
	if( listItem->getListIndex() + 1 < mListItems->size() )
	{
		for(unsigned int i = listItem->getListIndex() + 1; i < mListItems->size(); ++i)
		{
			/*
			float x = 0.f;

			HandbookListItemRef listItem = boost::dynamic_pointer_cast<HandbookListItem>(selectedObject);
			if(listItem) { x = 10.0f; }
			*/

			HandbookListItemRef li = mListItems->at(i);
			li->setPosition(
				ci::Vec3f
				(
					0.0f,
					i * li->getSize()->value().y,
					0.f
				)
			);



		}
	}

	//set list item as closed
	listItem->setIsShowingSublist(false);
	mActiveListItem = NULL;

	//for testing
	onListCloseComplete();
}

void HandbookList::openSubList(HandbookListItemRef listItem)
{
	SubItemList subItemList = mListItemSubMap[listItem];
	unsigned int numSubItems = subItemList->size();
	
	//open up current list
	for(unsigned int i = 0; i < numSubItems; ++i)
	{
		subItemList->at(i)->onAnimateIn(0.1f + i/20.0f);
	}
	
	//move other list items down
	if( listItem->getListIndex() + 1 < mListItems->size() )
	{
		for(unsigned int i = listItem->getListIndex() + 1; i < mListItems->size(); ++i)
		{
			HandbookListItemRef li = mListItems->at(i);
			li->setPosition(
				ci::Vec3f
				(
					0.f,
					(numSubItems * subItemList->at(0)->getSize()->value().y) + li->getPosition()->value().y + SUB_LIST_PADDING,
					0.f
				)
			);
		}
	}

	//eventually animate - play sound for now
	SoundController::getInstance().playSound(SoundController::HB_SELECT_DROPDOWN);

	listItem->setIsShowingSublist(true);	
	
	mActiveListItem = listItem;
	mNextActiveListItem = NULL;

	//for testing
	onListOpenComplete();
}

void HandbookList::onListCloseComplete()
{
	updateSelectionList();

	if(mNextActiveListItem) 
		openSubList(mNextActiveListItem);
	else
		onListOpenComplete();
}

void HandbookList::onListOpenComplete()
{
	mIsAnimating = false;

	updateSelectionList();
}
