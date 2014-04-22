#pragma once

#include "ButtonView.h"
#include "TextView.h"
#include "ImageView.h"

class HandbookTabView : public ButtonView
{
protected:
	TextViewRef		text;
	ImageViewRef	lock; 
	bool			mLocked;

public:
	HandbookTabView(Assets* assets, std::string buttonText);
	virtual ~HandbookTabView(void);

	virtual void selectView(bool selected); 
	
	void setLocked(bool val); 
	bool getLocked(){ return mLocked; }; 
};
