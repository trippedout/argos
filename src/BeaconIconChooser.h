#pragma once

#include "InputView.h"
#include "BeaconIconButton.h"

class BeaconIconChooser : public InputView
{
protected:
	typedef boost::shared_ptr<BeaconIconButton> BeaconIconButtonRef;

	std::vector<BeaconIconButtonRef>*	mIcons;

public:
	BeaconIconChooser(Assets* assets);
	~BeaconIconChooser(void);
	
	void setActive(bool active);

	virtual void handleInput(INPUT_TYPE type);
	
	void playMovementSound(ci::Vec2i selection);
};

