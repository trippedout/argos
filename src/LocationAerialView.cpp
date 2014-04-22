#include "LocationAerialView.h"

using namespace ci;
using namespace ci::app;
using namespace ci::gl;

LocationAerialView::LocationAerialView(Assets* assets) : 
	LocationBaseView(assets),
	mLoop(NULL)
{
	//guide first!
	createGuide("guides/location_aerial.png");
}

LocationAerialView::~LocationAerialView(void)
{
	if(mLoop){
		this->removeChild(mLoop);
		mLoop.reset();
		mLoop = NULL; 
	}
}

void LocationAerialView::onTransitionStart()
{
	LocationBaseView::onTransitionStart();
	
	if(mLoop){
		mLoop->stop();	
	}
}

void LocationAerialView::onTransitionSwap(bool isNext)
{
	if(isNext)
	{
		if(!mLoop){
			ci::app::console() << " LocationAerialView::onTransitionSwap loading in video"<<std::endl;

			mLoop = ScrubVideoRef( new ScrubVideoObject(new ci::qtime::MovieGl( loadAsset("scenes/location/loops/aerial.mp4"))));
			mLoop->setLoop(0.f, mLoop->getDuration());
			mLoop->setScale(ci::Vec3f(1.5f, 1.5f, 0.f));
		}
	}
	else
	{
		this->removeChild(mLoop);
	}
}

void LocationAerialView::onTransitionEnd()
{
	LocationBaseView::onTransitionEnd();
	
	if(mLoop){
		mLoop->setAlpha(0.f);
		timeline().apply(mLoop->getAlpha(), 1.f, .5f)
			.startFn(boost::bind(&LocationAerialView::onAnimateInStart, this))
			.delay(.15f);
	}
}

void LocationAerialView::onAnimateInStart()
{
	if(mLoop){
		this->addChild(mLoop);	
		mLoop->setTime(0.f);
		mLoop->play();
	}
}

void LocationAerialView::disposeTempAssets()
{
	if(mLoop){
		ci::app::console() << " LocationAerialView::disposeTempAssets deleting video"<<std::endl;

		this->removeChild(mLoop);
		mLoop.reset(); 
		mLoop = NULL; 
	}
}

void LocationAerialView::handleInput(INPUT_TYPE type)
{

}

#ifdef _WIN32
void LocationAerialView::handleGamepad(XINPUT_GAMEPAD gamepad)
{

}
#endif