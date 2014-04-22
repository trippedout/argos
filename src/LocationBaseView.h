#pragma once

#include "InputView.h"

class LocationBaseView : public InputView
{
protected:
	bool		mIsTransitioning;
public:
	LocationBaseView(Assets* assets);
	virtual ~LocationBaseView(void);

	virtual void onTransitionStart();
	virtual void onTransitionSwap(bool isNext);
	virtual void onTransitionEnd();
};

