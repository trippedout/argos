#pragma once

#include "View.h"
#include "ScrubVideoObject.h"

class LocationTransitionView : public View
{

protected:
	ScrubVideoRef		mOrbitalToAerial, mAerialToOrbital;
	ScrubVideoRef		mAerialToLocal, mLocalToAerial;
	//ScrubVideoRef		mHandbookToOrbital;
	ScrubVideoRef		mCurrentTransition;
	bool				mFirstRun;
	int					mFrameCount;
	int					mFrameSwap;

	float				mHalfDuration;
	bool				mIsHalfway;


public:
	enum TransitionType { 
		ORB_AERIAL, AERIAL_ORB, AERIAL_LOCAL, LOCAL_AERIAL, LOCAL_ORBITAL, ORBITAL, AERIAL, LOCAL
	};

	enum TransitionEvent { 
		TRANNY_SWAP, TRANNY_END,
	};

	LocationTransitionView(Assets* assets);
	~LocationTransitionView(void);

	virtual void onAnimateIn();

	virtual void update(float dt);
	virtual void render();
	void startTransition(TransitionType type);
	

	typedef boost::signals2::signal<void(TransitionEvent)> TransitionSignal;
	TransitionSignal	mTransitionSignal;
	TransitionSignal*	getTransitionSignal() { return &mTransitionSignal; };
};

