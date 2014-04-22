#include "PrefsViewToggleButton.h"


PrefsViewToggleButton::PrefsViewToggleButton(Assets* assets) : View(assets)
{
	mOffBtn = ButtonViewRef(new ARGOSButtonView(assets, "OFF") );
	boost::dynamic_pointer_cast<ARGOSButtonView>(mOffBtn)->getTextView()->setFontSize(15);
	mOffBtn->selectView(true);

	mOnBtn	= ButtonViewRef(new ARGOSButtonView(assets, "ON") );
	boost::dynamic_pointer_cast<ARGOSButtonView>(mOnBtn)->getTextView()->setFontSize(15);
	mOnBtn->setPosition(ci::Vec3f(181,1,0));

	this->addChild(mOffBtn);
	this->addChild(mOnBtn);

	off = true;
}


PrefsViewToggleButton::~PrefsViewToggleButton(void)
{
}

void PrefsViewToggleButton::toggle()
{
	mOffBtn->selectView(false);
	mOnBtn->selectView(false);

	if(off)
		mOnBtn->selectView(true);
	else
		mOffBtn->selectView(true);

	off = !off;
}
	
bool PrefsViewToggleButton::isOff()
{
	return off;
}