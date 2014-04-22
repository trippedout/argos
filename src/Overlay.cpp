#include "Overlay.h"


Overlay::Overlay()
{
	fullScreen = false;
	active = false;
}

Overlay::~Overlay(void)
{

}

void Overlay::setFullScreen(bool val)
{
	fullScreen = val;
}

bool Overlay::getFullScreen()
{
	return fullScreen;
}

void Overlay::setActive(bool val)
{
	active = val;
}

bool Overlay::getActive()
{
	return active;
}

void Overlay::initView(Assets* assets)
{
	view = new OverlayView(assets);
}

OverlayView* Overlay::getView()
{
	return view; 
}