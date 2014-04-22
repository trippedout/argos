#pragma once

#include "InputView.h"
#include "TextButtonView.h"
#include "PrefsViewToggleButton.h"
#include "PrefsViewLanguageList.h"

class PrefsView : public InputView
{
protected:
	typedef boost::shared_ptr<PrefsViewToggleButton> PrefsViewToggleButtonRef;
	typedef boost::shared_ptr<PrefsViewLanguageList> PrefsLanguageList;

	TextViewRef			mTitle;
	
	std::map<std::string, ci::Vec3f>	mAnimMap;

	ButtonViewRef			mSubtitles;
	ButtonViewRef			mClosedCap;
	ButtonViewRef			mNotifs;
	ButtonViewRef			mSocialNotifs;
	ButtonViewRef			mVoiceCtrls;
	ButtonViewRef			mAutoShare;

	std::map<ButtonViewRef, PrefsViewToggleButtonRef> mToggleMap;

	PrefsViewToggleButtonRef	mToggleSubtitles;
	PrefsViewToggleButtonRef	mToggleClosedCap;
	PrefsViewToggleButtonRef	mToggleNotifs;
	PrefsViewToggleButtonRef	mToggleSocialNotifs;
	PrefsViewToggleButtonRef	mToggleVoiceCtrls;
	PrefsViewToggleButtonRef	mToggleAutoShare;

	PrefsLanguageList	mLanguageList;

	ImageViewRef			mSelectBack;

public:
	PrefsView(Assets* assets);
	~PrefsView(void);
	
	virtual void	render();
	virtual void	draw();

	virtual void	moveSelection(ci::Vec2i dir);
	virtual void	handleInput(INPUT_TYPE type);

	virtual void	onAnimateIn();
	virtual void	onAnimateOut();
	virtual void	resetPositions();

protected:
	virtual void		toggleButton();
	virtual void		showLanguageList();
	virtual void		hideLanguageList();
	virtual void		activateLanguageList(bool activate);

};

