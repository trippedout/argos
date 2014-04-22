#include "BackgroundDOFParticle.h"

using namespace ci;
using namespace ci::app;

BackgroundDOFParticle::BackgroundDOFParticle(ci::Vec4f startingPos) : ParticleBase(startingPos)
{
	reset();
}

BackgroundDOFParticle::~BackgroundDOFParticle(void)
{

}

void BackgroundDOFParticle::reset()
{
	mPosition = mStartingPosition;
	mPosition.w = Rand::randFloat() + 1;

	mVelocity = ci::Vec4f(-Rand::randFloat() * 2 + .5f, Rand::randFloat() * .2f - .1f, -Rand::randFloat() * 0.75f, 0);
}

void BackgroundDOFParticle::update()
{
	//speed up as the
	mVelocity.x -= Rand::randFloat() * 0.005f;

	if(mPosition.x < 1500)
		mVelocity.y += (Rand::randFloat() * 0.2f - 0.1f) * -((mPosition.x - 1200.0f)/1200.0f);

	mPosition += mVelocity;

	if(mPosition.x <= 50) 
	{
		reset();
	}
}
