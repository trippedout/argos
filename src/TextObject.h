#pragma once
#include "ImageView.h"
#include "cinder/Text.h"

class TextObject : public ImageView
{
protected: 
	ci::TextLayout textLayout; 

public:
	TextObject(std::string text, ci::Font* font, ci::ColorA color);
	~TextObject(void);
};