#pragma once

#include "View.h"
#include "Particle.h"
#include "Resources.h"

#include "BackgroundDOFParticle.h"

#include "cinder/gl/GlslProg.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"

#include "cinder/params/Params.h"

//using namespace ci::gl;

class BackgroundMainParticles :	public View
{
	typedef boost::shared_ptr<BackgroundDOFParticle> DOFParticle;
public:
	BackgroundMainParticles(Assets* assets);
	~BackgroundMainParticles(void);

	virtual void update(float dt);
	virtual void render(); 

protected:
	//shader
	ci::gl::GlslProgRef			mShader;

	ci::gl::Texture*			mParticleTexture;
	ci::Vec4f*				mPositions;
	int					mNumParticles;
	int					mMaxParticles;
	float				mPhase;
    ci::Perlin				mPerlin;

	std::vector<DOFParticle>	mParticles;

	//testing
    ci::params::InterfaceGlRef		mParams;
	ci::Vec3f						mTranslate;
	ci::Vec3f						mRotate;
};

