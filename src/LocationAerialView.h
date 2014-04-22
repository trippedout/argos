#pragma once

#include "LocationBaseView.h"
#include "ScrubVideoObject.h"

class LocationAerialView : public LocationBaseView
{
protected:
	ScrubVideoRef		mLoop;
public:
	LocationAerialView(Assets* assets);
	~LocationAerialView(void);

	void onAnimateInStart();

	virtual void onTransitionStart();
	virtual void onTransitionSwap(bool isNext);
	virtual void onTransitionEnd();
	
	virtual void handleInput(INPUT_TYPE type);
#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamepad);
#endif

	virtual void disposeTempAssets();
};

