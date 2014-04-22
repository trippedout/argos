#include "ExpandableListView.h"


ExpandableListView::ExpandableListView(Assets* assets) : ListView(assets)
{

}

ExpandableListView::~ExpandableListView(void)
{

}

void ExpandableListView::addItem(ButtonViewRef item)
{
	//add item to list
	mListItems.push_back(item);

	//add item to display list
	int listIndex = mListItems.size()-1;
	item->setPosition(ci::Vec3f(0,item->getSize()->value().y * listIndex,0));
	this->addChild(item);

	mTotalItems = (int)mListItems.size();
}

void ExpandableListView::selectItem(int index)
{
	this->deselectAll();

	if(index > -1)
	{
		mListItems[index]->selectView(true);
		mCurrentIndex = index;
	}
}

void ExpandableListView::handleInput(INPUT_TYPE type)
{
	ci::app::console() << "ListView::handleInput()" << std::endl;

	switch(type)
	{
	case LEFT_STICK_UP:
		mCurrentIndex--;
		if(mCurrentIndex < 0 ) mCurrentIndex = 0;
		selectItem(mCurrentIndex);
		break;

	case LEFT_STICK_DOWN:
		mCurrentIndex++;
		if(mCurrentIndex >= mTotalItems ) mCurrentIndex = mTotalItems - 1;
		selectItem(mCurrentIndex);
		break;
            
        default:
            break;
	}
}
