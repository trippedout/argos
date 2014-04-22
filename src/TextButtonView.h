#pragma once

#include "ButtonView.h"
#include "TextView.h"

typedef boost::shared_ptr<class TextButtonView> TextButtonViewRef;

class TextButtonView : public ButtonView
{
public:
	TextButtonView(Assets* assets, std::string buttonText);
	virtual ~TextButtonView(void);

	virtual void selectView(bool selected); 

protected:
	TextViewRef		text1;
	TextViewRef		text2;
};

