#pragma once

#include "InputView.h"

#include "ImageView.h"
#include "VideoObject.h"
#include "TextButtonView.h"

#include "SoundController.h"

class StartView : public InputView
{
	ImageViewRef		title;
	TextButtonViewRef	play;
	TextButtonViewRef	handbook;
	TextButtonViewRef	prefs;
	TextButtonViewRef	host; 

	ImageViewRef		bgBox; 

	VideoViewRef		mIntro;

public:
	StartView(Assets* assets);

	virtual void	onAnimateIn(Settings::ViewID from);
	virtual void	onAnimateOut(Settings::ViewID to);

	virtual void	handleInput(INPUT_TYPE type);

	void startIntro();
	void onStartIntroComplete();
	void selectOld();

	~StartView(void);
};

