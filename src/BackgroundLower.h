#pragma once

#include "BackgroundBase.h"
#include "DataFloorView.h"
#include "BackgroundSmallParticleSystem.h"
#include "BackgroundLowerParticles.h"

#include "ImageView.h"

#include "cinder/Rand.h"

class BackgroundLower :	public BackgroundBase
{
	typedef boost::shared_ptr<DataFloorView> DataFloorViewRef;
	typedef boost::shared_ptr<BackgroundSmallParticleSystem> ParticleSystemRef;
	typedef boost::shared_ptr<BackgroundLowerParticles> LowerParticlesRef;
protected:
	ImageViewRef			mBeam;
	ci::gl::Texture					mNoise;

	ParticleSystemRef	mSmallParticles;
	LowerParticlesRef	mFloatingParticles;
	DataFloorViewRef	mDataFloor;
public:
	BackgroundLower(Assets* assets);
	~BackgroundLower(void);

	virtual void handleInputFromView();

	virtual void update(float dt);

	virtual void onAnimateIn();
	virtual void onAnimateOut();

	virtual void prerender();
	virtual void render(); 
};

