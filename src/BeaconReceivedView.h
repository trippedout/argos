#pragma once

#include "BeaconSubView.h"

#include "BeaconReceivedListItem.h"
#include "BeaconData.h"

class BeaconReceivedView : public BeaconSubView
{
protected:
	typedef boost::shared_ptr<BeaconReceivedListItem> BeaconReceivedListItemRef;
	typedef std::vector<BeaconDataRef>* BeaconVec;
	typedef std::vector<BeaconReceivedListItemRef>* BeaconListItems;

	BeaconVec			mBeacons;
	BeaconListItems		mBeaconListItems;
	
	BeaconReceivedListItemRef	mNextToOpen;

public:
	BeaconReceivedView(Assets* assets);
	~BeaconReceivedView(void);

	void updateBeacons(BeaconVec beacons);

	bool closeOpenItems();
	void closeItem(BeaconReceivedListItemRef item);
	void onCloseItem();

	void openItem(BeaconReceivedListItemRef item);
	void onOpenItem();

	virtual void setSelection(ci::Vec2i selection);
	virtual void setActive(bool active);
	virtual void handleInput(INPUT_TYPE type);
};

