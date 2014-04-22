#pragma once

#include "ButtonView.h"
#include "ImageView.h"
#include "TextView.h"

typedef boost::shared_ptr<class ARGOSButtonView> ARGOSButtonViewRef;

class ARGOSButtonView :	public ButtonView
{
protected: 
	ImageViewRef bg1; 
	ImageViewRef bg2; 
	ImageViewRef bg3; 

	TextViewRef text; 

public:
	ARGOSButtonView(Assets* assets, std::string copy);
	virtual ~ARGOSButtonView(void);

	virtual void	selectView(bool select);
	
	TextViewRef		getTextView();
};