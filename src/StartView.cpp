#include "StartView.h"

using namespace ci;
using namespace ci::app;

StartView::StartView(Assets* assets) : 
	InputView(assets)
{
	setPosition(ci::Vec3f(473,434,0)); 
	
	title = ImageViewRef( new ImageView(new ci::gl::Texture( loadImage( loadAsset( "scenes/start/title_full.png" ) ) ) ) );
	title->setPosition(ci::Vec3f(-28,-22,0));

	play =  TextButtonViewRef(new TextButtonView(assets, "START EPISODE"));
	handbook = TextButtonViewRef(new TextButtonView(assets, "HANDBOOK"));
	host =  TextButtonViewRef(new TextButtonView(assets, "HOST/JOIN PARTY"));
	prefs = TextButtonViewRef(new TextButtonView(assets, "PREFERENCES"));

	play->setPosition(ci::Vec3f(0,0,0)); 
	handbook->setPosition(ci::Vec3f(0,50,0)); 
	host->setPosition(ci::Vec3f(0,100,0)); 
	prefs->setPosition(ci::Vec3f(0,150,0)); 

	play->setCenter(ci::Vec3f(1.0f,0.5f,0)); 
	handbook->setCenter(ci::Vec3f(1.0f,0.5f,0)); 
	host->setCenter(ci::Vec3f(1.0f,0.5f,0)); 
	prefs->setCenter(ci::Vec3f(1.0f,0.5f,0));

	bgBox = ImageViewRef( new ImageView(assets->getTextureByID("bgBox")) );
	bgBox->setScale(ci::Vec3f(1920.0f/bgBox->getSize()->value().x, 1080.0f/bgBox->getSize()->value().y,0));
	bgBox->setPosition(ci::Vec3f(-473,-434,0)); 
	bgBox->setAlpha(0);
	
	this->addChild(title);
	this->addChild(bgBox);
	this->addChild(play);
	this->addChild(handbook);
	this->addChild(prefs);
	this->addChild(host);

	int num_of_col = 1;
	int num_of_row = 4;
	selectionGrid.resize( num_of_col , std::vector<ButtonViewRef>( num_of_row , NULL ) );

	// hmm better way? maybe hard coding is the safest... not sure. 
	selectionGrid[0][0] = play; 
	selectionGrid[0][1] = handbook; 
	selectionGrid[0][2] = host; 
	selectionGrid[0][3] = prefs; 
	
	this->deselectAll();
	useSelection = true; 
	moveSelection(ci::Vec2i(0,0));
	
	mIntro	=	VideoViewRef( new VideoObject(assets->getVideoByID("start_intro")) );
	mIntro->setPosition(ci::Vec3f(-473,-434,0));
	this->addChild(mIntro);
	mIntro->play();
	mIntro->stop();
}

StartView::~StartView(void)
{

}

void StartView::startIntro()
{
	mIntro->play();

	SoundController::getInstance().playSound(SoundController::START_ENTER);

	timeline().apply(mIntro->getAlpha(), 0.f, .9f)
		.delay(2.5f)
		.finishFn(boost::bind(&StartView::onStartIntroComplete, this));
}

void StartView::onStartIntroComplete()
{
	mIntro->stop();
	this->removeChild(mIntro);

	SoundController::getInstance().startBackgroundLoop();

	mIntro.reset();
	mIntro = NULL;
}

void StartView::onAnimateIn(Settings::ViewID from)
{
	if(from == Settings::ViewID::HOST || from == Settings::ViewID::PREFS ){
		// something? 
	}else{
		bgBox->setAlpha(1.0f);
	}

	timeline().apply(bgBox->getAlpha(), 0.0f, 0.4f, EaseOutQuint()).finishFn( std::bind( &StartView::onAnimateInComplete, this ) );

	timeline().apply(play->getAlpha(), 1.0f, 0.25f, EaseOutQuint());
	timeline().apply(handbook->getAlpha(), 1.0f, 0.25f, EaseOutQuint()).delay(0.05f);
	timeline().apply(host->getAlpha(), 1.0f, 0.25f, EaseOutQuint()).delay(0.1f);
	timeline().apply(prefs->getAlpha(), 1.0f, 0.25f, EaseOutQuint()).delay(0.15f);
}

void StartView::onAnimateOut(Settings::ViewID to)
{
	float a = 0; 
	if( to == Settings::ViewID::PLAY || to == Settings::ViewID::HANDBOOK){
		a=1.0f;
	}

	timeline().apply(bgBox->getAlpha(), a, 0.4f, EaseInQuint()).finishFn( std::bind( &StartView::onAnimateOutComplete, this ) );
	
	timeline().apply(play->getAlpha(), 0.0f, 0.25f, EaseOutQuint());
	if(to != Settings::ViewID::HANDBOOK) timeline().apply(handbook->getAlpha(), 0.0f, 0.25f, EaseOutQuint()).delay(0.05f);
	if(to != Settings::ViewID::HOST) timeline().apply(host->getAlpha(), 0.0f, 0.25f, EaseOutQuint()).delay(0.1f);
	if(to != Settings::ViewID::PREFS) timeline().apply(prefs->getAlpha(), 0.0f, 0.25f, EaseOutQuint()).delay(0.15f);
}

void StartView::handleInput(INPUT_TYPE type)
{	
	switch(type)
	{
	case DPAD_UP:
	case UP:
		this->moveSelection(ci::Vec2i(0,-1));
		break;
		
	case DPAD_DOWN:
	case DOWN:
		this->moveSelection(ci::Vec2i(0,1));
		break;

	case SELECT:
		SoundController::getInstance().playSound(SoundController::SELECT);

		if(selectedObject == play) signal(this, Settings::ViewID::PLAY);
		else if(selectedObject == host) signal(this, Settings::ViewID::HOST);
		else if(selectedObject == prefs)  signal(this, Settings::ViewID::PREFS);
		else if(selectedObject == handbook) signal(this, Settings::ViewID::HANDBOOK);
		break;

	//specifc voice handling
	case PRESSED_X:
	case HANDBOOK:
		signal(this, Settings::ViewID::HANDBOOK);
		break;

	case START_EPISODE:
		signal(this, Settings::ViewID::PLAY);
		break;

	case HOST_A_PARTY:
		signal(this, Settings::ViewID::HOST);
		break;

	case PREFERENCES:
		signal(this, Settings::ViewID::PREFS);
		break;
	}	
}

void StartView::selectOld()
{
	 
}