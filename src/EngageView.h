#pragma once

#include "InputView.h"
#include "ImageView.h"
#include "ScrubVideoObject.h"
#include "ARGOSButtonView.h"

#include "EngageGameView.h"
#include "VideoSnapshot.h"
#include "Overlay.h"

class EngageView : public InputView
{
protected:
	typedef boost::shared_ptr<EngageGameView> EngageGameViewRef;

	ImageViewRef		mBackground;
	ImageViewRef		mStartText;
	ImageViewRef		mSelectBack;

	VideoSnapshotRef	mSnapshot; 

	ImageViewRef		mBeam, mOniEmpty1, mOniEmpty2, mOniEmpty3, mOni1, mOni2, mOni3;
	void hideAllOnis();

	ARGOSButtonViewRef	mStartBtn;
	ARGOSButtonViewRef	mRetryBtn;

	ScrubVideoRef		mOniFail;
	ImageViewRef		mFailBg;

	ScrubVideoRef		mEnter;
	ImageViewRef		mEnterBg;

	EngageGameViewRef	mGameView;
	float				mMaxGameBlur;

	bool				mIsPlaying;
	FMOD::Channel*		mTimerChannel;

	void onGameResult(EngageGameView::GameResult result);
	void showFail();
	void showStart();
	void showRetry();
	void startGame();
	
	void retryGame();

	void fadeOutStart();

public:
	EngageView(VideoSnapshotRef	snapshot, Assets* assets);
	~EngageView(void);
	
	virtual void prerender(); 
	virtual void update(float dt);
	
	virtual void onAnimateIn(Settings::ViewID from);
	virtual void onAnimateOut(Settings::ViewID to);

	//virtual void onAnimateIn(){ onAnimateIn(Settings::ViewID::PLAY); };
	//virtual void onAnimateOut(){ onAnimateOut(Settings::ViewID::PLAY); };

	virtual void	handleInput(INPUT_TYPE type);
};

