#pragma once

#include "InputView.h"

#include "HandbookListItem.h"
#include "HandbookListSubItem.h"

#include "HBCategory.h"
#include "DataNode.h"
#include "LocationNode.h"


class HandbookList : public InputView
{
protected:
	static const int SUB_LIST_PADDING = 15;

	typedef boost::shared_ptr<HandbookListItem> HandbookListItemRef;
	typedef boost::shared_ptr<HandbookListSubItem> HandbookListSubItemRef;
	
	typedef boost::signals2::signal<void(DataNodeRef)> SubItemClickedSignal;
	typedef boost::signals2::signal<void(LocationNodeRef)> LocationItemClickedSignal;
	typedef boost::signals2::signal<void(int)> SubItemSelectedSignal;

	typedef std::vector<HBCategoryRef>* CategoryList;
	typedef std::vector<HandbookListItemRef>* ItemList;
	typedef std::vector<HandbookListSubItemRef>* SubItemList;

	CategoryList			mCategories;
	unsigned int			mNumCategories;

	ItemList				mListItems;
	HandbookListItemRef		mActiveListItem;
	HandbookListItemRef		mNextActiveListItem;
	
	bool					mIsAnimating;

	std::map<HandbookListItemRef, SubItemList> mListItemSubMap;
	
	SubItemClickedSignal		mSubItemClickedSignal;
	LocationItemClickedSignal	mLocationItemClickedSignal;
	SubItemSelectedSignal		mSubItemSelectedSignal;

	virtual void buildListItems();
	virtual void updateData();

	virtual void updateSelectionList();
	virtual void toggleListItem(HandbookListItemRef listItem);
	virtual void closeSubList(HandbookListItemRef listItem);
	virtual void openSubList(HandbookListItemRef listItem);

	virtual void onListCloseComplete();
	virtual void onListOpenComplete();

	virtual void moveSelection(ci::Vec2i dir);

public:
	HandbookList(Assets* assets, CategoryList categories);
	HandbookList(Assets* assets);
	~HandbookList(void);

	virtual void onAnimateIn();
	virtual void onAnimateOut();

	virtual void handleInput(INPUT_TYPE type);
	virtual bool closeOpenLists();

	SubItemClickedSignal*	getSubItemClickedSignal() { return &mSubItemClickedSignal; };
	LocationItemClickedSignal*	getLocationItemClickedSignal() { return &mLocationItemClickedSignal; };
	SubItemSelectedSignal*	getSubItemSelectedSignal() { return &mSubItemSelectedSignal; };
};

