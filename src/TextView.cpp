#include "TextView.h"

TextView::TextView(std::string text, ci::Font* font, ci::ColorA color) : ImageView(), textLayout(NULL)
{
	//store font
	mText = text;
	mFont = font;
	mColor = color;

	setText(text);
}

TextView::~TextView(void)
{
	deleteTexture();
}

void TextView::setFontSize(float fontSize)
{
	float currentSize = mFont->getSize();
	float diff = fontSize / currentSize;

	this->setScale(ci::Vec3f(diff,diff,0));
}

void TextView::setText(std::string text)
{
	if(textLayout != NULL)
	{
		deleteTexture();

		delete textLayout;
		textLayout = NULL;
	}

	textLayout = new ci::TextLayout;
	textLayout->setFont(*mFont);
	textLayout->setColor(mColor);
	textLayout->addLine(text);
	
	setTexture(new ci::gl::Texture(textLayout->render(true)));
}

std::string TextView::getText()
{
	return mText;
}