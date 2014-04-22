#pragma once

#include "View.h"
#include "ImageView.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Camera.h"
#include "ParticleBase.h"

class LocationOrbitalOverlay : public View
{
	typedef boost::shared_ptr<class SphereParticleGroup> ParticleGroupRef;
protected:
	ci::gl::GlslProgRef		mShader;
    ci::gl::Fbo*			mFbo;
	ci::gl::VboMesh*		mSphereVbo; 
	ci::TriMesh*		mSphereMesh;
	ImageViewRef	mSphereAlpha;

	ci::Anim<ci::Vec3f> sphereRotation;

	//points on sphere
	void					drawParticles();
	std::pair<ci::Vec3f*, ci::Vec3f*>	getRandomPoints();

	ImageViewRef				mParticleTex;
	int							mMaxParticleGroups;
	std::vector<ParticleGroupRef>*	mParticleGroups;
	
	ci::gl::GlslProgRef				mParticleShader;

	ci::Vec4f*					mPositions;
	int						mNumParticles;
	int						mMaxParticles;

	ci::Vec4f*					mExtraData;


public:
	LocationOrbitalOverlay(Assets* assets);
	~LocationOrbitalOverlay(void);

	virtual void update(float dt);
	virtual void prerender();
	virtual void render();
	virtual void draw();

	void drawQuad();

	ci::Vec3f sphereOffset;
	float sphereScale;
	float drawDistance;
};

class SphereParticleGroup 
{
	typedef boost::shared_ptr<class SphereParticle> ParticleRef;
protected:
	ci::Vec3f*				mStart;
	ci::Vec3f*				mEnd;
	ci::Vec3f				mDistance, mLength;

	float					mStartingLife;
	float					mLife;
	float					mLifeAlphaBuffer;
	float					mAlpha;
	
	std::vector<ParticleRef>*	mParticles;
	bool					mIsDead;
public:
	static const int	mNumPoints = 12;

	SphereParticleGroup(std::pair<ci::Vec3f*, ci::Vec3f*> start_end);
	~SphereParticleGroup(void);

	void update();
	bool getIsDead() { return mIsDead; };
	ci::Vec3f* getStart() { return mStart; };
	ci::Vec3f* getEnd() { return mEnd; };

	ci::Vec4f getPosition(int particle_index);
	float getSeed(int particle_index);

	float getAlpha() { return mAlpha; };
};


class SphereParticle : public ParticleBase
{
	float				mSeed;
	float				mIncrease;
public:
	SphereParticle(ci::Vec4f start, ci::Vec4f velocity);
	~SphereParticle(void);

	virtual void reset();
	virtual void update();

	ci::Vec4f getPosition() { return mPosition; };
	float getSeed() { return mSeed; };
};