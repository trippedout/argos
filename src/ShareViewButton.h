#pragma once

#include "ButtonView.h"
#include "ImageView.h"
#include "TextView.h"

class ShareViewButton :	public ButtonView
{
protected: 
	ImageViewRef bg1, bg2, bg3;
	ImageViewRef highlight;

	TextViewRef text; 

public:
	ShareViewButton(Assets* assets, std::string copy);
	virtual ~ShareViewButton(void);

	virtual void	selectView(bool select);
	
	TextViewRef		getTextView();
};