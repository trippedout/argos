#pragma once

#include "ButtonView.h"

#include "TextView.h"
#include "ImageView.h"

class HandbookListItem : public ButtonView
{
protected:
	TextViewRef			mText;
	ImageViewRef		mDivider;

	ImageViewRef		bg1; 
	ImageViewRef		bg2; 
	ImageViewRef		bg3; 

	ImageViewRef		lock; 

	bool				mIsShowingSublist, mLocked;
	int					mListIndex;

public:
	HandbookListItem(Assets* assets, std::string title);
	~HandbookListItem(void);
	
	virtual void selectView(bool select);
	virtual void render();
	virtual void onAnimateIn(); 

	void setIsShowingSublist(bool showing) { mIsShowingSublist = showing; };
	bool getIsShowingSublist() { return mIsShowingSublist; };
	
	void setListIndex(int index) { mListIndex = index; };
	int  getListIndex() { return mListIndex; };

	void setLocked(bool val); 
	bool getLocked(){ return mLocked; }; 

};

