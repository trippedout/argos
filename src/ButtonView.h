#pragma once

#include "View.h"
#include "SelectableView.h"

typedef boost::shared_ptr<class ButtonView> ButtonViewRef;

class ButtonView : public View, public SelectableView 
{

public:
	ButtonView(Assets* assets);
	virtual ~ButtonView(void);

	virtual void selectView(bool select);
	virtual void activate(); 
	virtual void deactivate(); 
};

