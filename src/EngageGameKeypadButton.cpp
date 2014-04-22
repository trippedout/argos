#include "EngageGameKeypadButton.h"

using namespace ci::app;

EngageGameKeypadButton::EngageGameKeypadButton(Assets* assets, KeyType keyType, const bool isCharacterOnly) :
	ButtonView(assets),
	mKeyType(keyType),
	mIsCharacterOnly(isCharacterOnly),
	mCircle( ImageViewRef( new ImageView( assets->getTextureByID("eng_keypad_circle") ) ) ),
	mCircle2( ImageViewRef( new ImageView( assets->getTextureByID("eng_keypad_circle") ) ) ),
	mCircle3( ImageViewRef( new ImageView( assets->getTextureByID("eng_keypad_circle") ) ) ),
	mCircleGlow( ImageViewRef( new ImageView( assets->getTextureByID("eng_keypad_circle_glow") ) ) ),
	mCharacter(NULL)
{
	init();
}

void EngageGameKeypadButton::init()
{
	//set character	
	std::string character = "eng_keypad_";

	switch(mKeyType)
	{
	case ONE: character.append("1"); break;
	case TWO: character.append("2"); break;
	case THREE: character.append("3"); break;
	case FOUR: character.append("4"); break;
	case FIVE: character.append("5"); break;
	case SIX: character.append("6"); break;

	case DOWN:
	case LEFT:
	case RIGHT:
		character.append("arrow");
		break;
	}

	mCharacter = ImageViewRef( new ImageView( mAssets->getTextureByID(character) ) );

	//arrows default down
	if(mKeyType == LEFT)
	{
		mCharacter->setRotation(ci::Vec3f(0.f,0.f, 90.f));
		mCharacter->setPosition(ci::Vec3f(106.f,0.f,0.f));
	}
	
	if(mKeyType == RIGHT)
	{
		mCharacter->setRotation(ci::Vec3f(0.f,0.f, -90.f));
		mCharacter->setPosition(ci::Vec3f(0.f,106.f,0.f));
	}

	
	//final setup
	if( mIsCharacterOnly )	
		this->setScale(ci::Vec3f(.7f, .8f, 1.f));
	else
	{
		mCircle2->setPosition(ci::Vec3f(0,0,40));
		mCircle3->setPosition(ci::Vec3f(0,0,-40)); 

		mCircle2->setAlpha(0);
		mCircle3->setAlpha(0);
		mCircleGlow->setAlpha(0);
		
		this->addChild(mCircleGlow);
		this->addChild(mCircle2);
		this->addChild(mCircle);
		this->addChild(mCircle3);
		
		this->setAlpha(0.85f);
	}

	this->addChild(mCharacter);
}

EngageGameKeypadButton::~EngageGameKeypadButton(void)
{
	mCircle.reset();
	mCharacter.reset();
}

void EngageGameKeypadButton::selectView(bool select)
{
	if(select == selected) return; 

	ButtonView::selectView(select); 

	if(selected){
		timeline().apply(getAlpha(), 1.0f, 0.1f);

		timeline().apply(mCircle2->getAlpha(), 0.25f, 0.1f); 
		timeline().apply(mCircle3->getAlpha(), 0.25f, 0.1f); 
		timeline().apply(mCircleGlow->getAlpha(), 1.0f, 0.1f); 
		//this->setAlpha(1.f);
	}
	else{
		timeline().apply(getAlpha(), 0.85f, 0.2f); 

		timeline().apply(mCircle2->getAlpha(), 0.0f, 0.2f); 
		timeline().apply(mCircle3->getAlpha(), 0.0f, 0.2f); 
		timeline().apply(mCircleGlow->getAlpha(), 0.0f, 0.2f); 
		//this->setAlpha(0.85f);
	}
}