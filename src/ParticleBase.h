#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"

//using namespace ci;
//using namespace ci::app;
//using namespace std;

class ParticleBase
{
protected:
	ci::Vec4f	mStartingPosition;
	ci::Vec4f	mPosition;
	ci::Vec4f	mVelocity;

public:
	ParticleBase(ci::Vec4f startingPos);
	virtual ~ParticleBase(void);

	virtual void	update();
	
	ci::Vec4f			getPosition() { return mPosition; };
};
