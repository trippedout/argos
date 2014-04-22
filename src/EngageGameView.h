#pragma once

#include "InputView.h"
#include "ImageView.h"
#include "TextView.h"
#include "cinder/Camera.h"

#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"

#include "EngageGameKeypadButton.h"

class EngageGameView : public InputView
{
public:
	enum GameResult{
		GAME_WIN, GAME_LOSE
	};

protected:
	typedef boost::shared_ptr<EngageGameKeypadButton> KeypadButtonRef;
	typedef EngageGameKeypadButton::KeyType KeyType;
	typedef boost::signals2::signal<void(GameResult)> GameResultSignal;

	ImageViewRef		mDashes;
	ImageViewRef		mOutline1, mOutline1Select1, mOutline1Select2;
	ImageViewRef		mOutline2, mOutline2Select1, mOutline2Select2;

	TextViewRef			mTimerText;
	TextViewRef			mTimerStaticText;
	TextViewRef			mTitleText;

	bool				selectTop;
	bool				mIsPlaying;
	float				mTimerCount;
	std::string			getTimerFormatted(float time);

	DisplayObjectContainerRef	mLetterContainer;
	int							mTotalLettersPlayed;
	int							mResult;
	std::vector<KeypadButtonRef>*	mGameLetters;
	void setGameLetter(KeyType type);
	void checkGameResults();

	GameResultSignal			mGameResuleSignal;

	KeypadButtonRef		mOne, mTwo, mThree, mFour, mFive, mSix, mLeft1, mLeft2, mDown1, mDown2, mRight1, mRight2;
	
	void createKeypad();
	KeypadButtonRef getKey(EngageGameKeypadButton::KeyType keyType);
	
	//drawing!
	ci::gl::GlslProgRef		mShader;
    ci::gl::Fbo				mFbo1;
	ci::gl::Fbo				mFbo2;
	ci::Anim<float>		mBlurAmount;
	ci::Anim<float>		mAlphaAmount;

public:
	EngageGameView(Assets* assets);
	~EngageGameView(void);

	void resetGame();
	void startGame();

	virtual void update(float dt);
	virtual void render();
	virtual void prerender(); 

	virtual void moveSelection(ci::Vec2i dir);
	virtual void handleInput(INPUT_TYPE type);

	ci::Anim<float>* getBlurAmount() { return &mBlurAmount; };
	ci::Anim<float>* getAlphaAmount() { return &mAlphaAmount; };

	GameResultSignal* getGameResuleSignal() { return &mGameResuleSignal; };
};

