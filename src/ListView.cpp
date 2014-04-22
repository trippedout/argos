#include "ListView.h"

using namespace ci;
using namespace ci::app;

ListView::ListView(Assets* assets) : 
	InputView(assets),
	mCurrentIndex(-1),
	mTotalItems(0)
{
	this->deselectAll();
}

ListView::~ListView(void)
{

}

void ListView::addItem(ButtonViewRef item)
{
	//add item to list
	mListItems.push_back(item);

	//add item to display list
	int listIndex = mListItems.size()-1;
	item->setPosition(ci::Vec3f(0,item->getSize()->value().y * listIndex,0));
	this->addChild(item);

	mTotalItems = (int)mListItems.size();
}

void ListView::selectItem(int index)
{
	this->deselectAll();

	if(index > -1)
	{
		mListItems[index]->selectView(true);
		mCurrentIndex = index;
	}
}

void ListView::handleInput(INPUT_TYPE type)
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

int ListView::getHeight()
{
	if(mTotalItems > 0)
		return (int)( mTotalItems * mListItems[0]->getSize()->value().y ); 
	else 
		return 0;
}