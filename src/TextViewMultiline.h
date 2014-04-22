#pragma once

#include "InputView.h"
#include "ImageView.h"
#include "cinder/Text.h"
#include <cmath>


class TextViewMultiline : public InputView
{
protected:
	ImageViewRef	mGuide;

	ci::Vec2i			mTextSize;
	ci::Vec2i			mMaskSize;
	ci::gl::Texture			mTextTexture;
	ImageViewRef	mImage;
    ci::TextBox			mTextBox;

	ImageViewRef	mTextImageView;
	ImageViewRef	mArrow;

	ci::Font*			mFont;
	ci::ColorA			mColor;
	bool			snapToLine; 

public:
	TextViewMultiline(Assets* assets, std::string text, ci::Vec2i maskSize, ci::Font* font, ci::ColorA color);
	TextViewMultiline(Assets* assets, std::string text, ci::Vec2i maskSize);
	TextViewMultiline(Assets* assets, std::string text);
	~TextViewMultiline(void);

	virtual void handleInput(INPUT_TYPE type);
    
#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamepad);
#endif
	
	void setText(const std::string &text);

	bool scrollable(); 
	void project3D(bool val){ mTextImageView->project3D(val); }; 

private:
	//for overloaded constructors
	void init(Assets* assets, std::string text, ci::Vec2i maskSize, ci::Font* font, ci::ColorA color);

	void checkMasking();
};

