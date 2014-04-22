#pragma once

#include "BackgroundBase.h"
#include "BackgroundMainGlow.h"
#include "BackgroundMainParticles.h"
#include "BackgroundSmallParticleSystem.h"

class BackgroundMain : public BackgroundBase
{
protected:
	DisplayObjectRef			layerGlow;
	DisplayObjectRef			layerParticles;
	SmallParticleSystemRef		layerSmallParticles;

public:
	BackgroundMain(Assets* assets);
	~BackgroundMain(void);

	virtual void handleInputFromView();

	virtual void update(float dt);
	virtual void render();
};

