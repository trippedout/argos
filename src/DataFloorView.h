#pragma once

#include "DisplayObject.h"

#include "cinder/TriMesh.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"

#include "cinder/Camera.h"
#include "cinder/Rand.h"

#include "cinder/ObjLoader.h"
#include "cinder/ImageIo.h"

#include "Assets.h"

class DataFloorView : public DisplayObject
{
	typedef boost::shared_ptr<class FloorVertice> FloorVerticeRef;
protected:
	ci::gl::GlslProgRef		mShader;
	ci::TriMesh*		mFloorMesh;
	ci::gl::VboMesh*		mFloorVbo;
    ci::gl::Fbo*			mFloorFbo;
	ci::gl::Texture*		mOverlay;

	std::vector<FloorVerticeRef>*	mVertices;

	float mRotationSpeed;

	//circles
	ci::Vec2f*				mInnerCirclePositions;
	ci::Vec2f*				mMiddleCirclePositions;
	ci::Vec2f*				mOuterCirclePositions;
	ci::Vec2f*				mLargestCirclePositions;
	int					mNumCirclePoints;

	void renderCircles();


public:
	DataFloorView(Assets* assets);
	~DataFloorView(void);

	void prerender();
	void update(float dt);
	void render();
	void drawQuad();

	void animateIn();
	void animateOut();
	
	ci::Vec3f			mFloorOffset;
	ci::Vec3f			mFloorRotation;
	ci::Vec3f			mFloorScale;
	float			mFOV;
	
	ci::Vec3f			mPos;
	float			mRot1, mRot2, mRot3, mRot4;
	ci::Vec3f			mScale;
};


class FloorVertice 
{
	ci::Vec3f mStartingPosition, mPosition, mVelocity;
	ci::Anim<float> mZOffset;
	float mOffScreen;

public:
	FloorVertice(ci::Vec3f pos) : mStartingPosition(pos), mPosition(pos), mVelocity(ci::Vec3f()), mOffScreen(-300.f), mZOffset(mOffScreen) {} ;
	~FloorVertice(void) {};
	
	void update();
	void animateIn(float delay);
	void animateOut(float delay);

	void setVelocity(ci::Vec3f velocity) { mVelocity = velocity; };
	ci::Vec3f* getPosition() { return &mPosition; };
};