#include "TextViewMultiline.h"

using namespace ci;

TextViewMultiline::TextViewMultiline(Assets* assets, std::string text) : InputView( assets )
{
	ci::Font* font = assets->getFontByID("data_overview_body");
	ci::ColorA color = ci::ColorA(1.0f,1.0f,1.0f,1.0f);
	ci::Vec2i maskSize = ci::Vec2i(600,426);

	init(assets, text, maskSize, font, color);
}

TextViewMultiline::TextViewMultiline(Assets* assets, std::string text, ci::Vec2i maskSize) : InputView( assets )
{
	ci::Font* font = assets->getFontByID("data_overview_body");
	ci::ColorA color = ci::ColorA(1.0f,1.0f,1.0f,1.0f);

	init(assets, text, maskSize, font, color);
}

TextViewMultiline::TextViewMultiline(Assets* assets, std::string text, ci::Vec2i maskSize, ci::Font* font,ci::ColorA color) : InputView( assets )
{
	init(assets, text, maskSize, font, color);
}

TextViewMultiline::~TextViewMultiline(void)
{
	mTextImageView.reset();
	mTextImageView = NULL; 
}

void TextViewMultiline::init(Assets* assets, std::string text, ci::Vec2i maskSize, ci::Font* font,ci::ColorA color)
{
	snapToLine = false;

	if(text == "") text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. \
					Morbi feugiat porta nunc ut accumsan. Etiam sagittis ultricies laoreet.";

	mFont = font;
	mColor = color;
	mMaskSize = maskSize;

	mTextBox = TextBox()
		.alignment(TextBox::LEFT)
		.font(*mFont)
		.color(mColor)
		.size(mMaskSize.x,TextBox::GROW)
		.text(text);

	mTextSize = mTextBox.measure();


	if(scrollable()) 
	{
		//set size
		setSize( ci::Vec3f((float)mMaskSize.x, (float)mMaskSize.y, 0) ); 
		
		//add arrow
		//mArrow = ImageViewRef( new ImageView( mAssets->getTextureByID("data_item_arrow") ) );
		//mArrow->setCenter(ci::Vec3f(0.5f,0.f,0.f));
		//mArrow->setPosition(ci::Vec3f( mMaskSize.x * .5f, mMaskSize.y + 20.f, 0.f ) );
		//this->addChild(mArrow);
	}
	else
	{
		setSize( ci::Vec3f((float)mTextSize.x, (float)mTextSize.y, 0) ); 
	}

	//ci::app::console() << "TextViewMultiline::init() maskSize: " << mMaskSize << " textSize: " << mTextSize << std::endl;

	mTextTexture = mTextBox.render();
	mTextImageView = ImageViewRef(new ImageView( &mTextTexture ));

	//set render rect so that we can mask it
	mTextImageView->setRenderRect(ci::Rectf(0,0,(float)mMaskSize.x, (float)mMaskSize.y));


	this->addChild(mTextImageView);
}

void TextViewMultiline::setText(const std::string &text)
{
	mTextBox.setText(text);
	mTextSize = mTextBox.measure();
	mTextTexture = mTextBox.render();
	mTextImageView->setRenderRect(ci::Rectf(0,0,(float)mMaskSize.x, (float)mMaskSize.y));
	
	if(scrollable()) {
		setSize( ci::Vec3f((float)mMaskSize.x, (float)mMaskSize.y, 0) ); 
	}else{
		setSize( ci::Vec3f((float)mTextSize.x, (float)mTextSize.y, 0) ); 
	}
}

void TextViewMultiline::handleInput(INPUT_TYPE type)
{
}

#ifdef _WIN32
void TextViewMultiline::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	if( !scrollable() ) return; 

	int max = 32000; //ish? 
	ci::Vec2f left = ci::Vec2f((float)gamepad.sThumbLX/(float)max, gamepad.sThumbLY/(float)max ); 
		
	// dpad overrides the left stick. 
	int dpadUp = (int)(gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
	int dpadDown = (int)(gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
	
	if(  dpadUp > 0 ) {
		left.y = 0.75f; 
	} else if( dpadDown > 0 ) {
		left.y = -0.75f; 
	}

	if( left.y > 0.25f || left.y < -0.25f )
	{
		mTextImageView->getRenderRect()->y1 -= left.y * 10.f;
		checkMasking();
		snapToLine = true;
		return;
	}else{
		if(snapToLine){
			snapToLine = false;

			float height = mTextImageView->getRenderRect()->y1;
			float lineHeight = mTextBox.getFont().getSize() + mTextBox.getFont().getLeading() + mTextBox.getFont().getDescent();
			
			int segments = floor( height / lineHeight + 0.5f); //floor + 0.5f is a cheap 'round' 
			
			float y = lineHeight * segments;

			//ci::app::console() << "TextViewMultiline::handleGamepad  font: "<<  mTextBox.getFont().getSize() << " / leading: "<< mTextBox.getFont().getLeading() << " / descent: " << mTextBox.getFont().getDescent() << " / ascent"<< mTextBox.getFont().getAscent() <<std::endl; 
			
			mTextImageView->getRenderRect()->y1 = y;
		}
	}
}
#endif

bool TextViewMultiline::scrollable()
{
	return mMaskSize.y < mTextSize.y;
}

void TextViewMultiline::checkMasking()
{
	if( mTextImageView->getRenderRect()->y1 < 0 ) 
	{
		mTextImageView->getRenderRect()->y1 = 0;
		//mArrow->setAlpha(1.f);
	}
	else if( mTextImageView->getRenderRect()->y1 > mTextSize.y - mMaskSize.y )
	{
		mTextImageView->getRenderRect()->y1 = (float)mTextSize.y - (float)mMaskSize.y;
		//mArrow->setAlpha(0.f);
	}
}