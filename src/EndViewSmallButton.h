#pragma once

#include "ButtonView.h"
#include "ImageView.h"
#include "TextView.h"

class EndViewSmallButton : public ButtonView
{
protected: 
	ImageViewRef bg1; 
	ImageViewRef bg2; 
	ImageViewRef bg3; 

	TextViewRef text; 

public:
	EndViewSmallButton(Assets* assets, std::string copy);
	~EndViewSmallButton(void);


	virtual void	selectView(bool select);
	
	TextViewRef		getTextView();
};
