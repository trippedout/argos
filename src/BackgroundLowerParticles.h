#pragma once

#include "View.h"
#include "ParticleBase.h"

#include "cinder/gl/GlslProg.h"

class BackgroundLowerParticles : public View
{
	typedef boost::shared_ptr<class BackgroundLowerParticle> ParticleRef;

protected:
	ci::gl::GlslProgRef				mShader;
	std::vector<ParticleRef>*	mParticles;
	ci::Vec4f*					mPositions;
	int						mNumParticles;
	int						mMaxParticles;

public:
	BackgroundLowerParticles(Assets* assets);
	~BackgroundLowerParticles(void);

	virtual void update();
	virtual void render();
};

class BackgroundLowerParticle : public ParticleBase
{
protected:
	float w, h;
public:
	BackgroundLowerParticle(void);
	~BackgroundLowerParticle(void);

	virtual void reset();
	virtual void update();
};
