#include "BackgroundMain.h"

BackgroundMain::BackgroundMain(Assets* assets) : 
	BackgroundBase(assets),
	layerGlow(DisplayObjectRef(new BackgroundMainGlow(assets))),
	layerParticles(DisplayObjectRef(new BackgroundMainParticles(assets))),
	layerSmallParticles(SmallParticleSystemRef(new BackgroundSmallParticleSystem(assets, 7000)))
{
	this->addChild(layerGlow);
	this->addChild(layerParticles);

	layerSmallParticles->setPosition(ci::Vec3f(840,1520,0));
	layerSmallParticles->setRotation(ci::Vec3f(0,0,-90));
	this->addChild(layerSmallParticles);
}

BackgroundMain::~BackgroundMain(void)
{
	layerGlow.reset();
	layerParticles.reset();
	layerSmallParticles.reset();
}

void BackgroundMain::update(float dt)
{
	layerGlow->update(dt);
	layerParticles->update(dt);
	layerSmallParticles->update();
}

void BackgroundMain::render()
{
	BackgroundBase::render();
}

void BackgroundMain::handleInputFromView()
{
	//eventually do cool shit
}
