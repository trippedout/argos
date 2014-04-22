#pragma once

#include "InputView.h"

#include "HandbookTabs.h"
#include "HandbookList.h"
#include "HandbookLocationList.h"
#include "HandbookBackgroundView.h"

typedef boost::shared_ptr<class HandbookNavigationView> HandbookNavigationViewRef;

class HandbookNavigationView : public InputView
{
protected:
	typedef boost::shared_ptr<HandbookTabs> HandbookTabsRef;
	typedef boost::shared_ptr<HandbookList> HandbookListRef;
	typedef boost::signals2::signal<void(DataNodeRef)> DataSelectedSignal;
	typedef boost::signals2::signal<void(LocationNodeRef)> LocationSelectedSignal;
	
	HandbookBackgroundViewRef mBackground;
	HandbookTabsRef		mTabsView;

	HandbookListRef		mItemsList;
	HandbookListRef		mCharactersList;
	HandbookListRef		mLocationsList;
	HandbookListRef		mOniList;
	HandbookListRef		mCurrentList;

	std::map<HandbookTabs::Tab, HandbookListRef> mTabListMap;

	DataSelectedSignal	mDataSelectedSignal;
	LocationSelectedSignal mLocationSelectedSignal;

	void onTabChange(HandbookTabs::Tab tab);
	void onSubItemClicked(DataNodeRef dataNode);
	void onLocationItemClicked(LocationNodeRef locationNode);
	void onSubItemSelected(int val);

public:
	HandbookNavigationView(HandbookBackgroundViewRef background, Assets* assets);
	~HandbookNavigationView(void);

	virtual void render();
	virtual void handleInput(INPUT_TYPE type);
	
	virtual void onAnimateIn(); 
	virtual void onAnimateOut(); 

	void showFirstTab(); 
	
	void showOniTab();

	DataSelectedSignal* getDataSelectedSignal() { return &mDataSelectedSignal; };
	LocationSelectedSignal* getLocationSelectedSignal() { return &mLocationSelectedSignal; };

};

