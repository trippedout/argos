#pragma once

#include "View.h"
#include "ARGOSButtonView.h"

class PrefsViewToggleButton : public View
{
protected:
	ButtonViewRef		mOffBtn;
	ButtonViewRef		mOnBtn;

	bool			off;

public:
	PrefsViewToggleButton(Assets* assets);
	~PrefsViewToggleButton(void);

	virtual void	toggle();
	
	bool			isOff();

};

