#pragma once

#include "ButtonView.h"
#include "ImageView.h"
#include "TextView.h"

class BeaconTabView : public ButtonView
{
protected: 
	ImageViewRef bg_on, bg_off;

	TextViewRef text_on, text_off; 

public:
	BeaconTabView(Assets* assets, std::string copy);
	~BeaconTabView(void);

	virtual void	selectView(bool select);
};
