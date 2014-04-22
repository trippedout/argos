#pragma once

#include "View.h"
#include "BackgroundSmallParticle.h"

#include "cinder/gl/GlslProg.h"

class BackgroundSmallParticleSystem : public View
{
	typedef boost::shared_ptr<BackgroundSmallParticle> SmallParticle;
protected:
	ci::gl::GlslProgRef				mShader;

	std::vector<SmallParticle>	mParticles;
	ci::Vec4f*					mPositions;
	int						mNumParticles;
	int						mMaxParticles;
	float					mPhase;
	
public:
	BackgroundSmallParticleSystem(Assets* assets, int numParticles);
	~BackgroundSmallParticleSystem(void);

	virtual void update();
	virtual void render();
};

typedef boost::shared_ptr<BackgroundSmallParticleSystem> SmallParticleSystemRef;
