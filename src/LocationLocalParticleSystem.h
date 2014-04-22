#pragma once

#include "View.h"
#include "ParticleBase.h"

#include "cinder/gl/GlslProg.h"

class LocationLocalParticleSystem : public View
{
	typedef boost::shared_ptr<class LocationSmallParticle> Particle;
	typedef boost::shared_ptr<class LocationBigParticle> BigParticle;
protected:
	ci::gl::GlslProgRef				mShader;

	std::vector<Particle>*		mParticles;
	ci::Vec4f*					mPositions;
	int						mNumParticles;
	int						mMaxParticles;
	
	ci::gl::GlslProgRef				mBigShader;
	std::vector<BigParticle>*	mBigParticles;
	ci::Vec4f*					mBigPositions;
	int						mNumBigParticles;
	int						mMaxBigParticles;
	
public:
	LocationLocalParticleSystem(Assets* assets);
	~LocationLocalParticleSystem(void);
	
	virtual void update();
	virtual void render();
};

class LocationSmallParticle : public ParticleBase
{
protected:
	int		mWidth;
	float	mHalfWidth;

public:
	LocationSmallParticle(ci::Vec4f startingPos);
	~LocationSmallParticle(void);

	virtual void reset();
	virtual void update();
};

class LocationBigParticle : public ParticleBase
{
protected:
	float w, h;
public:
	LocationBigParticle(void);
	~LocationBigParticle(void);

	virtual void reset();
	virtual void update();
};

