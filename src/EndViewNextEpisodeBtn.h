#pragma once

#include "ButtonView.h"
#include "ImageView.h"
#include "TextView.h"

class EndViewNextEpisodeBtn : public ButtonView
{
protected:
	ImageViewRef			mBg;
	TextViewRef				mText;
public:
	EndViewNextEpisodeBtn(Assets* assets);
	~EndViewNextEpisodeBtn(void);

	virtual void selectView(bool select);
};

