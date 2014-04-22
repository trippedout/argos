#pragma once

#include "InputView.h"
#include "ShareViewButton.h"

class ShareView : public InputView
{
protected:
	typedef boost::shared_ptr<ShareViewButton> ShareViewButtonRef;

	ShareViewButtonRef	mWaypoint, mFacebook, mTwitter;

	ImageViewRef	mTitle, mSelectBack;

	bool		mIsShowing;

public:
	ShareView(Assets* assets);
	~ShareView(void);

	virtual void onAnimateIn();
	virtual void onAnimateOut();

	virtual void handleInput(INPUT_TYPE type);

	bool getIsShowing() { return mIsShowing; };
};

