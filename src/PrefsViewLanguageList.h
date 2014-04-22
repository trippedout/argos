#pragma once

#include "InputView.h"

#include "ARGOSButtonView.h"

class PrefsViewLanguageList : public InputView
{
protected:
	static const int	mNumLanguages = 13;
	
	TextViewRef			mCurrentLanguage;
	ImageViewRef		mCurrentLanguageBg;
	
	std::vector<ButtonViewRef>	mLanguages;
	bool				showing;
	
	void				setButtonAlphas(float alpha);

public:
	PrefsViewLanguageList(Assets* assets);
	~PrefsViewLanguageList(void);

	virtual void		show();
	virtual void		hide();
	virtual void		setActive(bool active); 
	virtual void		setLanguage(); 

	bool				isShowing();

	virtual void		handleInput(INPUT_TYPE type);

};

