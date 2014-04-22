#pragma once

#include "BeaconSubView.h"

#include "BeaconSubmitButton.h"
#include "BeaconIconChooser.h"
#include "BeaconMessageChooser.h"

class BeaconAddNewView : public BeaconSubView
{
protected:
	typedef boost::shared_ptr<BeaconIconChooser> BeaconIconChooserRef;
	typedef boost::shared_ptr<BeaconMessageChooser> BeaconMessageChooserRef;

	ImageViewRef				mHeader, mAddIconBg;
	BeaconIconChooserRef		mIconChooser;
	
	BeaconMessageChooserRef		mMessageChooser;

	ButtonViewRef				mSubmitBtn;
	
public:
	BeaconAddNewView(Assets* assets);
	~BeaconAddNewView(void);

	virtual void handleInput(INPUT_TYPE type);


	virtual void setActive(bool active);
};

