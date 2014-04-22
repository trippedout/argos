#include "EngageView.h"

using namespace ci;
using namespace ci::app;


EngageView::EngageView(VideoSnapshotRef	snapshot, Assets* assets) : 
	InputView(assets),
	mSnapshot(snapshot),
	mBackground( ImageViewRef( new ImageView( assets->getTextureByID("eng_bg") ) ) ),
	mOniFail( NULL ),
	mEnter( NULL ),
	mFailBg( ImageViewRef( new ImageView( assets->getTextureByID("eng_fail_bg") ) ) ),
	mGameView( EngageGameViewRef( new EngageGameView(assets) ) ),
	mEnterBg( ImageViewRef( new ImageView( assets->getTextureByID("eng_enter_bg") ) ) ),
	//mStartText( ImageViewRef( new ImageView( assets->getTextureByID("eng_start_text") ) ) ),
	mStartBtn( ARGOSButtonViewRef( new ARGOSButtonView(assets, "ENTER CODE") ) ),
	mRetryBtn( ARGOSButtonViewRef( new ARGOSButtonView(assets, "RETRY?") ) ),
	mOniEmpty1( ImageViewRef( new ImageView( assets->getTextureByID("eng_oni_empty") ) ) ),
	mOniEmpty2( ImageViewRef( new ImageView( assets->getTextureByID("eng_oni_empty") ) ) ),
	mOniEmpty3( ImageViewRef( new ImageView( assets->getTextureByID("eng_oni_empty") ) ) ),
	mOni1( ImageViewRef( new ImageView( assets->getTextureByID("eng_oni_1") ) ) ),
	mOni2( ImageViewRef( new ImageView( assets->getTextureByID("eng_oni_2") ) ) ),
	mOni3( ImageViewRef( new ImageView( assets->getTextureByID("eng_oni_3") ) ) ),
	mSelectBack( ImageViewRef( new ImageView( assets->getTextureByID("select_back") ) ) ),
	mBeam( ImageViewRef( new ImageView( assets->getTextureByID("hb_beam") ) )), 
	mMaxGameBlur(0.55f),
	mIsPlaying(false)
{
	//background
	this->addChild(mBackground);

	this->addChild(mSnapshot);

	mBeam->setAdditiveBlending(true);
	mBeam->setPosition(ci::Vec3f(0,520.0f,0)); 
	mBeam->setCenter(ci::Vec3f(0.5f, 1.0f, 0)); 
	mBeam->setRotation(ci::Vec3f(0,0,90.0f)); 
	mBeam->setScale(ci::Vec3f(1.0f, 2.0f, 0)); 
	mBeam->setAlpha(0);
	this->addChild(mBeam);

	//game
	mGameView->getGameResuleSignal()->connect(boost::bind(&EngageView::onGameResult, this, boost::arg<1>::arg()));
	mGameView->setPosition(ci::Vec3f( 770.f, 102.f, 0.f ));
	mGameView->getBlurAmount()->value() = mMaxGameBlur;
	this->addChild(mGameView);

	//fail oni
	mFailBg->setCenter(ci::Vec3f(0.5f, 0.5f, 0.f));
	mFailBg->setPosition(ci::Vec3f(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f - 30.f, 0.f));
	mFailBg->setAlpha(0.f);
	this->addChild(mFailBg);

	mEnterBg->setCenter(ci::Vec3f(.5f,0.f,0.f));
	mEnterBg->setPosition(ci::Vec3f(960.f,250.f,0.f));
	mEnterBg->setAlpha(0.f);
	this->addChild(mEnterBg);

	mStartBtn->setPosition(ci::Vec3f(850.f,850.f,0.f));
	mStartBtn->setAlpha(0.f);
	this->addChild(mStartBtn);

	//retry
	mRetryBtn->setPosition(ci::Vec3f(880.f,850.f,0.f));
	mRetryBtn->setAlpha(0.f);
	this->addChild(mRetryBtn);

	//oni symbols
	ci::Vec3f centerVec = ci::Vec3f(0.f, 0.5f, 0.f);
	ci::Vec3f vec1 = ci::Vec3f(1675.f,110.f,0.f);
	ci::Vec3f vec2 = ci::Vec3f(1675.f,160.f,0.f);
	ci::Vec3f vec3 = ci::Vec3f(1675.f,210.f,0.f);

	mOni1->setCenter(ci::Vec3f(centerVec));
	mOni1->setPosition(ci::Vec3f(vec1));
	this->addChild(mOni1);

	mOni2->setCenter(ci::Vec3f(centerVec));
	mOni2->setPosition(ci::Vec3f(vec2));
	this->addChild(mOni2);

	mOni3->setCenter(ci::Vec3f(centerVec));
	mOni3->setPosition(ci::Vec3f(vec3));
	this->addChild(mOni3);

	mOniEmpty1->setCenter(ci::Vec3f(centerVec));
	mOniEmpty1->setPosition(ci::Vec3f(vec1));
	this->addChild(mOniEmpty1);

	mOniEmpty2->setCenter(ci::Vec3f(centerVec));
	mOniEmpty2->setPosition(ci::Vec3f(vec2));
	this->addChild(mOniEmpty2);

	mOniEmpty3->setCenter(ci::Vec3f(centerVec));
	mOniEmpty3->setPosition(ci::Vec3f(vec3));
	this->addChild(mOniEmpty3);

	mSelectBack->setPosition(ci::Vec3f(180,950,0));
	mSelectBack->setAlpha(0);
	this->addChild(mSelectBack);
}

EngageView::~EngageView(void)
{
	mBackground.reset();
	//mOniFail->reset();
}

void EngageView::onAnimateIn(Settings::ViewID from)
{
	ci::app::console() << "EngageView::onAnimateIn() Handbook? " << from << "," << Settings::ViewID::HANDBOOK << std::endl;

	if(!mEnter){
		//start 
		mEnter =  ScrubVideoRef( new ScrubVideoObject(new ci::qtime::MovieGl( loadAsset("scenes/engage/enter.mp4"))));
		mEnter->setCenter(ci::Vec3f(.5f,0.f,0.f));
		mEnter->setPosition(ci::Vec3f(960.f,250.f,0.f));
		mEnter->setAlpha(0.f);
		mEnter->setAdditiveBlending(true);
		this->addChild(mEnter);
	}

	SoundController::getInstance().setBgVolume(1.f);
	SoundController::getInstance().playSound(SoundController::ENG_ENTER);

	timeline().apply(mRetryBtn->getAlpha(), 0.f, 0.f);
	mRetryBtn->selectView(false);

	//make sure game is cleared
	mGameView->resetGame();

	//get numbers
	int numUnlocked = mAssets->getData()->getUnlocked().onis;
	int maxOnis = mAssets->getData()->getUnlocked().onisTotal - 1;

	//set intro
	if(numUnlocked < maxOnis)
		showFail();
	else
		showStart();
	
	//set onis
	hideAllOnis();
	
	if(numUnlocked > 0)
	{
		timeline().apply(mOni1->getAlpha(), 1.0f, 0.25f, EaseOutQuint()).delay(0.5); 
		//mOni1->setAlpha(1.f);
		mOniEmpty1->setAlpha(0.f);
	}
	if(numUnlocked > 1)
	{
		timeline().apply(mOni2->getAlpha(), 1.0f, 0.25f, EaseOutQuint()).delay(0.6); 
		//mOni2->setAlpha(1.f);
		mOniEmpty2->setAlpha(0.f);
	}
	if(numUnlocked > 2)
	{
		timeline().apply(mOni3->getAlpha(), 1.0f, 0.25f, EaseOutQuint()).delay(0.7); 
		//mOni3->setAlpha(1.f);
		mOniEmpty3->setAlpha(0.f);
	}

	if(from == Settings::ViewID::PLAY){
		mSnapshot->updateSnapshot(); 
		mSnapshot->setBlur(0);
		mSnapshot->setAlpha(1);
		timeline().apply(mSnapshot->getAlpha(), 0.0f, 1.0f, EaseInQuint()); 
		timeline().apply(mSnapshot->getBlur(), 0.5f, 1.0f, EaseInQuint()); 
	}

	mBackground->setAlpha(0);
	timeline().apply(mBackground->getAlpha(), 1.0f, 1.0f, EaseOutQuint()); 

	mSelectBack->setAlpha(0);
	timeline().apply(mSelectBack->getAlpha(), 1.0f, 0.5f, EaseOutQuint()); 

	mBeam->setAlpha(0);
	timeline().apply(mBeam->getAlpha(), 1.0f, 0.25f, EaseOutQuint()); 
	timeline().appendTo(mBeam->getAlpha(), 0.75f, 0.25f, EaseOutQuint()); 


	timeline().apply(mGameView->getAlphaAmount(), 0.75f, 0.25f, EaseOutQuint()); 
	

	//finished!
	onAnimateInComplete();
}

void EngageView::hideAllOnis()
{
	mOni1->setAlpha(0.f);
	mOni2->setAlpha(0.f);
	mOni3->setAlpha(0.f);

	mOniEmpty1->setAlpha(1.f);
	mOniEmpty2->setAlpha(1.f);
	mOniEmpty3->setAlpha(1.f);
}

void EngageView::onAnimateOut(Settings::ViewID to)
{
	if(to == Settings::ViewID::PLAY){
		SoundController::getInstance().setBgVolume(0.f);
		timeline().apply(mSnapshot->getAlpha(), 1.0f, 1.0f, EaseInQuint()); 
		timeline().apply(mSnapshot->getBlur(), 0.0f, 1.0f, EaseInQuint()); 
	}else{
		SoundController::getInstance().setBgVolume(1.f);
	}
	
	timeline().apply(mGameView->getAlphaAmount(), 0.0f, 0.5f, EaseOutQuint()); 

	timeline().apply(mOniEmpty1->getAlpha(), 0.0f, 0.25f, EaseOutQuint()); 
	timeline().apply(mOniEmpty2->getAlpha(), 0.0f, 0.25f, EaseOutQuint()); 
	timeline().apply(mOniEmpty3->getAlpha(), 0.0f, 0.25f, EaseOutQuint()); 
	timeline().apply(mOni1->getAlpha(), 0.0f, 0.25f, EaseOutQuint()); 
	timeline().apply(mOni2->getAlpha(), 0.0f, 0.25f, EaseOutQuint()); 
	timeline().apply(mOni3->getAlpha(), 0.0f, 0.25f, EaseOutQuint()); 

	//mOniFail->stop();
	timeline().apply(mSelectBack->getAlpha(), 0.0f, 0.25f, EaseOutQuint()); 
	timeline().apply(mBeam->getAlpha(), 0.0f, 0.5f, EaseOutQuint()); 
	
	timeline().apply(mStartBtn->getAlpha(), 0.f, 0.25f);
	//timeline().apply(mEnter->getAlpha(), 0.f, 0.25f);
	timeline().apply(mEnterBg->getAlpha(), 0.f, 0.25f);
	//timeline().apply(mOniFail->getAlpha(), 0.f, 0.25f);
	timeline().apply(mFailBg->getAlpha(), 0.f, 0.25f);

	timeline().apply(mRetryBtn->getAlpha(), 0.f, 0.25f);
	mRetryBtn->selectView(false);

	//onAnimateOutComplete();
	timeline().apply(mBackground->getAlpha(), 0.0f, 1.0f, EaseOutQuint()).finishFn(boost::bind(&EngageView::onAnimateOutComplete,this));

	if(mEnter){
		removeChild(mEnter); 
		mEnter.reset(); 
		mEnter = NULL; 
	}

	if(mOniFail){
		removeChild(mOniFail); 
		mOniFail.reset(); 
		mEnter = NULL; 
	}
}

void EngageView::onGameResult(EngageGameView::GameResult result)
{
	mIsPlaying = false;

	//check timer and kill
	bool* isPlaying = new bool;
	mTimerChannel->isPlaying(isPlaying);
	if(*isPlaying) mTimerChannel->stop();
	delete isPlaying;

	//check results
	switch(result)
	{
	case EngageGameView::GameResult::GAME_LOSE:		
		showRetry();
		break;

	case EngageGameView::GameResult::GAME_WIN:
		SoundController::getInstance().playSound(SoundController::ENG_WIN);
		timeline().apply(mGameView->getBlurAmount(), mMaxGameBlur, .25f);
		mAssets->getData()->setEngageComplete(true);
		Overlay::getInstance().getView()->toast("ENGAGE"); 
		signal(this, Settings::ViewID::HANDBOOK);

		timeline().apply(mRetryBtn->getAlpha(), 0.f, 1.f);
		mRetryBtn->selectView(false);
		break;
	}
}

void EngageView::showStart()
{
	timeline().apply(mGameView->getAlphaAmount(), 0.75f, 0.25f, EaseOutQuint()); 

	if(mOniFail) mOniFail->setAlpha(0.f);
	mFailBg->setAlpha(0.f);
	mStartBtn->setAlpha(1.f);
	mStartBtn->selectView(false);

	if(mEnter){
		mEnter->setAlpha(1.f);
		mEnter->setTime(0.f);
		mEnter->play();
	}

	mEnterBg->setAlpha(1.f);
	
	timeline().apply(mSelectBack->getAlpha(), 1.0f, 0.25f, EaseOutQuint()); 
}

void EngageView::showRetry()
{
	timeline().apply(mGameView->getAlphaAmount(), 0.75f, 0.25f, EaseOutQuint()); 

	showFail();
	
	timeline().apply(mGameView->getBlurAmount(), mMaxGameBlur, .25f);
	timeline().apply(mRetryBtn->getAlpha(), 1.f, 1.f)
		.delay(1.f);

	mRetryBtn->selectView(false);
	timeline().apply(mSelectBack->getAlpha(), 1.0f, 0.25f, EaseOutQuint()); 
}

void EngageView::showFail()
{	
	timeline().apply(mGameView->getAlphaAmount(), 0.75f, 0.25f, EaseOutQuint()); 

	SoundController::getInstance().playSound(SoundController::ENG_FAIL);
	
	timeline().apply(mGameView->getBlurAmount(), mMaxGameBlur, .25f);

	if(!mOniFail){
		//fail
		mOniFail =  ScrubVideoRef( new ScrubVideoObject(new ci::qtime::MovieGl( loadAsset("scenes/engage/ONI_fail.mp4"))));
		mOniFail->setCenter(ci::Vec3f(0.5f, 0.5f, 0.f));
		mOniFail->setPosition(ci::Vec3f(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f - 30.f, 0.f));
		mOniFail->setAdditiveBlending(true);
		mOniFail->setAlpha(0.f);
		this->addChild(mOniFail);
	}

	if(mOniFail){
		mOniFail->setAlpha(1.f);
		mOniFail->setTime(0.f);
		mOniFail->play();
	}

	mFailBg->setAlpha(1.f);

	timeline().apply(mSelectBack->getAlpha(), 1.0f, 0.25f, EaseOutQuint()); 
}

void EngageView::update(float dt)
{
	InputView::update(dt);
	//do other shit
}

void EngageView::fadeOutStart()
{
	mStartBtn->selectView(true);

	if(mEnter) timeline().apply(mEnter->getAlpha(), 0.f, 0.5f);
	timeline().apply(mEnterBg->getAlpha(), 0.f, 0.5f);
	timeline().apply(mStartBtn->getAlpha(), 0.f, 0.5f)
		.delay(0.5f)	
		.finishFn(boost::bind(&EngageView::startGame,this));
}

void EngageView::startGame()
{
	//deselect the buttons. 
	mStartBtn->selectView(false);
	mRetryBtn->selectView(false);

	timeline().apply(mGameView->getAlphaAmount(), 1.0f, 0.25f, EaseOutQuint()); 

	SoundController::getInstance().playSound(SoundController::ENG_TIMER, &mTimerChannel);

	timeline().apply(mGameView->getBlurAmount(), 0.f, .25f);

	mGameView->resetGame();
	mGameView->startGame();
	mIsPlaying = true;
	
	timeline().apply(mSelectBack->getAlpha(), 0.0f, 0.25f, EaseOutQuint()); 
}

void EngageView::retryGame()
{
	if(mOniFail)mOniFail->setAlpha(0.f);
	//mRetryBtn->setAlpha(0.f);
	mFailBg->setAlpha(1.f);
	
	mRetryBtn->selectView(true);
	
	timeline().apply(mRetryBtn->getAlpha(), 0.f, 0.5f)
		.delay(0.5f)	
		.finishFn(boost::bind(&EngageView::startGame,this));

	//startGame();
}


void EngageView::prerender()
{
	mGameView->prerender();
}

void EngageView::handleInput(INPUT_TYPE type)
{
	if(mIsPlaying) 
	{
		mGameView->handleInput(type);
		return;
	}
	
	switch(type)
	{
	case BACK:
		if(!mIsPlaying) signal(this, Settings::ViewID::BACK);
		return;

	case SELECT:
		SoundController::getInstance().playSound(SoundController::ENG_SELECT);

		if(mStartBtn->getAlpha()->value() == 1.f)
			fadeOutStart();
		else if(mRetryBtn->getAlpha()->value() == 1.f)
			retryGame();
		break;
	}

	
}
