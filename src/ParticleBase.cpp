#include "ParticleBase.h"

using namespace ci;
using namespace ci::app;
using namespace std;

ParticleBase::ParticleBase(ci::Vec4f startingPos) :
	mStartingPosition(startingPos),
	mPosition(startingPos),
	mVelocity(ci::Vec4f())
{
	
}

ParticleBase::~ParticleBase(void)
{

}

void ParticleBase::update()
{

}