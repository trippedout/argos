#pragma once

#include "ButtonView.h"
#include "ImageView.h"

class EngageGameKeypadButton : public ButtonView
{
public:
	enum KeyType {
		ONE, TWO, THREE, FOUR, FIVE, SIX, LEFT, DOWN, RIGHT
	};

protected:
	ImageViewRef		mCircle;
	ImageViewRef		mCircle2;
	ImageViewRef		mCircle3;
	ImageViewRef		mCircleGlow;

	ImageViewRef		mCharacter;

	KeyType				mKeyType;

	bool				mIsCharacterOnly;

	void init();

public:
	EngageGameKeypadButton(Assets* assets, KeyType keyType, const bool isCharacterOnly);
	~EngageGameKeypadButton(void);
	
	virtual void selectView(bool select);

	KeyType	getKeyType() { return mKeyType; };
};

