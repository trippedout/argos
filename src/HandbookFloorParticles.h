#pragma once

#include "View.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Rand.h"

#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"

class HandbookFloorParticles : public View
{
protected:
	typedef boost::shared_ptr<class FloorVerticeParticle> VertParticle;
	typedef boost::shared_ptr<class FloorFloatingParticle> FloatParticle;

	std::vector<ci::Anim<ci::Vec3f>>*		mFloorVerts;
	unsigned int				mNumFloorVerts;
	ci::Vec3f*						mCenterParticle;

	ci::gl::GlslProgRef					mVertShader;
	ci::gl::Texture*					mVertTexture;

	std::vector<VertParticle>*		mVertParticles;
	ci::Vec4f*						mVertPositions;
	int							mNumVertParticles;
	int							mMaxVertParticles;
	
	ci::gl::GlslProgRef					mFloatShader;

	std::vector<FloatParticle>*		mFloatParticles;
	ci::Vec4f*						mFloatPositions;
	ci::Vec4f*						mFloatExtras;
	int							mNumFloatParticles;
	int							mMaxFloatParticles;

	ci::Vec3f getRandomPoint();
	
public:
	HandbookFloorParticles(Assets* assets, std::vector<ci::Anim<ci::Vec3f>>* floorVerts, int centerIndex);
	~HandbookFloorParticles(void);
	
	virtual void update();
	virtual void render();
};

class FloorVerticeParticle 
{
protected:
	ci::Vec3f*	mStartingPosition;
	ci::Vec4f	mPosition;
	ci::Vec4f	mVelocity;
	int		mLife;
	float	mIncrease;

public:
	FloorVerticeParticle(ci::Vec3f* startingPos);
	~FloorVerticeParticle(void);

	virtual void reset();
	virtual void update();

	ci::Vec4f getPosition() { return mPosition; };
};

class FloorFloatingParticle
{
protected:
	ci::Vec4f	mStartingPosition;
	ci::Vec4f	mPosition;
	ci::Vec4f	mVelocity;
	float	mStartingLife;
	float	mLife;
	float	mIncrease, mVertIncrease;
	int		mIndex;
public:
	FloorFloatingParticle(ci::Vec4f startingPos, int index);
	~FloorFloatingParticle(void);

	virtual void reset();
	virtual void update();

	ci::Vec4f getPosition() { return mPosition; };
	int getIndex() { return mIndex; };
	float getLife() { return mLife; };
};