#pragma once

#include "InputView.h"
#include "VideoObject.h"
#include "SoundController.h"

class PlayView :
	public InputView
{
public:
	PlayView(Assets* assets, VideoViewRef vid);
	~PlayView(void);

	virtual void	handleInput(INPUT_TYPE type);
#ifdef _WIN32
	virtual void	handleGamepad(XINPUT_GAMEPAD gamepad);
#endif

protected:

	float credits, videoDuration, inputDelay, frameSeekDelay, seekSpeed, rate; 
	VideoViewRef video; 
	bool exitingToEnd, fastforwarding, rewinding; 
	virtual void update(float dt);

	virtual void onAnimateIn();
	virtual void onAnimateOut();
};

