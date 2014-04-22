#pragma once

#include "InputView.h"
#include "BeaconReceivedListItem.h"

class BeaconMessageChooser : public InputView
{
protected:
	typedef boost::shared_ptr<BeaconReceivedListItem> BeaconReceivedListItemRef;
	typedef std::vector<BeaconReceivedListItemRef>* BeaconListItems;

	BeaconListItems		mMessages;
	void createMessages();

public:
	BeaconMessageChooser(Assets* assets);
	~BeaconMessageChooser(void);

	void setActive(bool active);

	virtual void handleInput(INPUT_TYPE type);
};

