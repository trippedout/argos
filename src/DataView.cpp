#include "DataView.h"

using namespace ci;
using namespace ci::app;

DataView::DataView(Assets* assets, VideoViewRef vid, VideoSnapshotRef snap, BackgroundController* bgController) :
	InputView(assets),
	mBgController(bgController)
{
	dataFBO = boost::shared_ptr< DataFBO >( new DataFBO(assets, vid) ); 
	video = vid;
	videoSnapshot = snap; 
	//dataFBO->setPosition(ci::Vec3f(350.0f, 140.0f,0));

	next = boost::shared_ptr< DataArrowButton >( new DataArrowButton(assets, true) );
	prev = boost::shared_ptr< DataArrowButton >( new DataArrowButton(assets, false) );

	next->setPosition(ci::Vec3f(1660.0f, 530.0f, 0)); 
	prev->setPosition(ci::Vec3f(211.0f, 530.0f, 0)); 

	bgBox = ImageViewRef( new ImageView(assets->getTextureByID("bgBox")) );
	bgBox->setScale(ci::Vec3f(1920.0f/bgBox->getSize()->value().x, 1080.0f/bgBox->getSize()->value().y,0));
	bgBox->setAlpha(0);
	
	mAddBack = ImageViewRef( new ImageView( assets->getTextureByID("select_back") ) );
	mAddBack->setPosition(ci::Vec3f(180,950,0));

	addChild( bgBox );
	addChild( videoSnapshot ); 
	addChild( dataFBO ); 
	addChild( next ); 
	addChild( prev ); 
	addChild( mAddBack );
}

void DataView::onAnimateIn()
{
	bgBox->setAlpha(1.0);
	timeline().apply(bgBox->getAlpha(), 0.0f, 0.5f, EaseInQuint());//.delay(0.5f);
	timeline().appendTo(bgBox->getAlpha(), 0.25f, 0.1f, EaseInQuint());
	timeline().appendTo(bgBox->getAlpha(), 0.0f, 0.1f, EaseOutQuint());
	timeline().appendTo(bgBox->getAlpha(), 0.1f, 0.1f, EaseInQuint());
	timeline().appendTo(bgBox->getAlpha(), 0.0f, 0.1f, EaseOutQuint());
	timeline().appendTo(bgBox->getAlpha(), 0.1f, 0.1f, EaseInQuint());
	timeline().appendTo(bgBox->getAlpha(), 0.0f, 0.2f, EaseOutQuint());

	videoSnapshot->updateSnapshot();
	videoSnapshot->setBlur(0);
	videoSnapshot->setAlpha(1.0f);
	//videoSnapshot->setPosition(ci::Vec3f(0,0,0)); 
	
	timeline().apply(videoSnapshot->getAlpha(), 0.4f, 1.0f, EaseOutQuint());
	timeline().apply(videoSnapshot->getBlur(), 0.5f, 1.0f, EaseOutQuint());
	timeline().apply(videoSnapshot->getPosition(), ci::Vec3f(0,0,50.0f), 1.0f, EaseOutQuint());

	mAddBack->setAlpha(0);
	timeline().apply(mAddBack->getAlpha(), 1.0f, 0.5f, EaseOutQuint()).delay(1.0f);

	mBgController->getCurrentBackground()->onAnimateIn();

	dataFBO->onAnimateIn();

	if(dataFBO->showRight()){
		next->show();
	}else{
		next->hide();
	}

	if(dataFBO->showLeft()){
		prev->show();
	}else{
		prev->hide();
	}
}

void DataView::onAnimateOut()
{
	dataFBO->onAnimateOut(); 
	//videoSnapshot
	
	mBgController->getCurrentBackground()->onAnimateOut();

	timeline().apply(videoSnapshot->getAlpha(), 1.0f, 1.0f, EaseInQuint());
	timeline().apply(videoSnapshot->getBlur(), 0.0f, 1.0f, EaseInQuint());
	timeline().apply(videoSnapshot->getPosition(), ci::Vec3f(0,0,0), 1.0f, EaseInQuint());

	timeline().apply(bgBox->getAlpha(), 1.0f, 1.0f, EaseOutQuint()).finishFn( boost::bind(&DataView::onAnimateOutComplete, this) );

	timeline().apply(mAddBack->getAlpha(), 0.0f, 1.0f, EaseOutQuint());
}

void DataView::prerender()
{
	dataFBO->prerender(); 
	//videoSnapshot->updateSnapshot(); 
}


void DataView::handleInput(INPUT_TYPE type)
{
	if(!Overlay::getInstance().getActive()){
		switch(type)
		{
		case NEXT:
			dataFBO->next();
			next->pulse();
			break;

		case PREVIOUS:
			dataFBO->prev();
			prev->pulse();
			break;

		case BACK:
			signal(this, Settings::ViewID::BACK);
			break;
		
		case PRESSED_X:
		case HANDBOOK:
			signal(this, Settings::ViewID::HANDBOOK);
			break;
		}
	}

	dataFBO->handleInput(type); 

	if(dataFBO->showRight()){
		next->show();
	}else{
		next->hide();
	}

	if(dataFBO->showLeft()){
		prev->show();
	}else{
		prev->hide();
	}
}

#ifdef _WIN32
void DataView::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	dataFBO->handleGamepad(gamepad);
}
#endif


DataView::~DataView(void)
{
}

