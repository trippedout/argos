#pragma once

#include "Assets.h"

#include "BackgroundBase.h"
#include "BackgroundMain.h"
#include "BackgroundLower.h"

#include "Settings.h"

//using namespace ci;

class BackgroundController
{
public:
	typedef boost::shared_ptr<BackgroundBase> BackgroundBaseRef;

	BackgroundController(Assets* assets);
	~BackgroundController(void);

	void update(float dt);
	void prerender();
	void render(); 
	
	void				changeView(Settings::ViewID id);
	BackgroundBaseRef	getCurrentBackground();

protected:

	BackgroundBaseRef	backgroundMain;
	BackgroundBaseRef	backgroundLower;

	BackgroundBaseRef	currentBackground;
};

