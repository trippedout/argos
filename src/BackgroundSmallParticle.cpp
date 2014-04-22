#include "BackgroundSmallParticle.h"

using namespace ci;
using namespace ci::app;

BackgroundSmallParticle::BackgroundSmallParticle(ci::Vec4f startingPos) : ParticleBase(startingPos)
{
	//set the seed value
	//
	reset();

	mWidth = getWindowWidth();
	mHalfWidth = mWidth * .5;
}


BackgroundSmallParticle::~BackgroundSmallParticle(void)
{
}

void BackgroundSmallParticle::reset()
{
	const float r = Rand::randFloat();
	mPosition = mStartingPosition;
	mPosition.x = (float)mHalfWidth + r * (Rand::randFloat() * 50.0f + 200.0f) - 100.0f;
	mPosition.z = 200 + r * 100;
	mPosition.w = Rand::randFloat();
	
	mEdge	=	200 + (Rand::randFloat() * 80 - 40);

	float xVel = (((float)mHalfWidth - mPosition.x) * 0.009f);
	mVelocity = ci::Vec4f(xVel,-Rand::randFloat() - .5f,0,0);
}

void BackgroundSmallParticle::update()
{
	//get new xVel
	float xVel = Rand::randFloat() * .2f - .1f;
	
	mVelocity.x += xVel;
	if( mVelocity.x > 1 ) mVelocity.x = 1;
	if( mVelocity.x < -1 ) mVelocity.x = -1;

	mVelocity.y -= Rand::randFloat() * 0.01f;
	mPosition += mVelocity;
	
	float distance = (float)mHalfWidth - mPosition.x;
	
	if(distance > 175 || distance < -175)
	{
		if(xVel > .1) mVelocity.x *= -.25;
		mPosition.z -= 2;
	}
		
			
	//update life
	mPosition.z -= 1;

	//if dead
	if(mPosition.z <= 0) reset();
}