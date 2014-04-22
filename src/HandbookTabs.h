#pragma once

#include "InputView.h"
#include "HandbookTabView.h"
#include "DataController.h"

class HandbookTabs : public InputView
{
public:
	enum Tab {
		ITEMS, CHARACTERS, LOCATIONS, ONI_FILES
	};

protected:
	typedef boost::shared_ptr<HandbookTabView> HandbookTabViewRef;
	typedef boost::signals2::signal<void(Tab)> TabChangeSignal;

	HandbookTabViewRef		mItemsTab;
	HandbookTabViewRef		mCharactersTab;
	HandbookTabViewRef		mLocationsTab;
	HandbookTabViewRef		mOniTab;

	std::map<HandbookTabViewRef, Tab>	mContentMap;
	std::map<HandbookTabViewRef, ci::gl::Texture*>	mTabBorderTexMap;
	std::map<HandbookTabViewRef, ci::gl::Texture*>	mTabSelectTexMap;

	ImageViewRef			mTabBorder;
	ImageViewRef			mTabSelect1;
	ImageViewRef			mTabSelect2;
	ImageViewRef			mTabSelect3;

	TabChangeSignal			mTabChangeSignal;

public:
	HandbookTabs(Assets* assets);
	~HandbookTabs(void);

	virtual void setSelection(ci::Vec2i selection);
	virtual void handleInput(INPUT_TYPE type);

	virtual void onAnimateIn(); 
	virtual void onAnimateOut(); 

	void showFirstTab();
	void showOniTab();
		
	TabChangeSignal*		getTabChangeSignal() { return &mTabChangeSignal; };
};

