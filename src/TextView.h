#pragma once
#include "ImageView.h"
#include "cinder/Text.h"

typedef boost::shared_ptr<class TextView> TextViewRef;

class TextView : public ImageView
{
protected: 
	ci::Font*			mFont;
	ci::ColorA			mColor;
	ci::TextLayout*		textLayout; 

    std::string			mText;

public:
	TextView(std::string text, ci::Font* font, ci::ColorA color);
	~TextView(void);

	virtual void setFontSize(float fontSize);
	virtual void setText(std::string text);
	
	std::string getText();
};
