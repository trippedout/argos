#pragma once

#include "ButtonView.h"
#include "ImageView.h"
#include "TextView.h"

#include "BeaconData.h"

class BeaconReceivedListItem : public ButtonView
{
	ImageViewRef		mBackgroundOff, mBackgroundOn;
	TextViewRef			mTextOff, mTextOn;
	
	ImageViewRef		mSubBackground;
	TextViewRef			mSubText;

	BeaconDataRef		mBeaconData;

	unsigned int		mListPosition;
	bool				mIsOpen;

	void init(std::string user, std::string message = "");

public:
	BeaconReceivedListItem(Assets* assets, BeaconDataRef beaconData);
	BeaconReceivedListItem(Assets* assets, std::string message);
	~BeaconReceivedListItem(void);
	
	void open();
	void close();

	virtual void selectView(bool select);

	void setListPosition(unsigned int pos) { mListPosition = pos; };
	unsigned int getListPosition() { return mListPosition; };

	bool getIsOpen() { return mIsOpen; };
};

