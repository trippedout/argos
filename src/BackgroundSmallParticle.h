#pragma once

#include "ParticleBase.h"

class BackgroundSmallParticle : public ParticleBase
{
protected:
	int		mLife;
	int		mId;
	float	mEdge;

	int		mWidth;
	double	mHalfWidth;

public:
	BackgroundSmallParticle(ci::Vec4f startingPos);
	~BackgroundSmallParticle(void);

	virtual void reset();
	virtual void update();
};

