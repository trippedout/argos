#include "BackgroundController.h"

using namespace ci;
using namespace ci::app;
using namespace std;

BackgroundController::BackgroundController(Assets* assets)
{
	ci::app::console() << "BackgroundController creation!" << std::endl;
	double startTime = getElapsedSeconds();

	backgroundMain = BackgroundBaseRef( new BackgroundMain(assets) );
	backgroundLower = BackgroundBaseRef( new BackgroundLower(assets) );

	currentBackground = backgroundMain;
	ci::app::console() << "BackgroundController loaded: " << getElapsedSeconds() - startTime << std::endl;
}

BackgroundController::~BackgroundController(void)
{
	backgroundMain.reset();
	backgroundLower.reset();

	if(currentBackground) currentBackground.reset();
}

void BackgroundController::update(float dt)
{
	if(currentBackground) currentBackground->update(dt);
}

void BackgroundController::prerender()
{
	if(currentBackground) currentBackground->prerender();
}

void BackgroundController::render()
{
	if(currentBackground) currentBackground->render();
}

void BackgroundController::changeView(Settings::ViewID id)
{
	switch(id)
	{
	case Settings::ViewID::START:
	case Settings::ViewID::PAUSE:
	case Settings::ViewID::PREFS:
	case Settings::ViewID::HOST:
	case Settings::ViewID::ANALYZE:
	case Settings::ViewID::END:
	case Settings::ViewID::BACK:
		currentBackground = backgroundMain;
		break;
		
	case Settings::ViewID::DATA:
		currentBackground = backgroundLower;
		break;
		
	case Settings::ViewID::HANDBOOK:
	case Settings::ViewID::LOCATION:
	case Settings::ViewID::BEACON:
	case Settings::ViewID::ENGAGE:
	case Settings::ViewID::PLAY:
		currentBackground = NULL;
		break;

	default:
		currentBackground = backgroundMain;
		break;
	}
}

BackgroundController::BackgroundBaseRef BackgroundController::getCurrentBackground()
{
	return currentBackground;
}