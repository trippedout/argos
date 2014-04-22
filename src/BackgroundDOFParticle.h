#pragma once

#include "ParticleBase.h"

class BackgroundDOFParticle : public ParticleBase
{
public:
	BackgroundDOFParticle(ci::Vec4f startingPos);
	~BackgroundDOFParticle(void);

	virtual void update();
	virtual void reset();
};
