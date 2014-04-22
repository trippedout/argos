#pragma once

#include "View.h"

#include "cinder/ObjLoader.h"
#include "cinder/Camera.h"

#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Color.h"

#include "cinder/Rand.h"

#include "HandbookFloorParticles.h"

#include "LightBeam.h"

class HandbookFloorView : public View
{
	typedef boost::shared_ptr<HandbookFloorParticles> FloorParticlesRef;

	void createFloorMesh();
	void createLightBeamMeshes();
	void createFBOs();

	void getNearestPoints(int index);
	bool checkIndice(int index);
	bool isIndexUsed(int index);
	void addPoint(int index);
	void checkPoint(int index, int closest);

protected:
	ci::gl::GlslProgRef		mFloorShader;
	ci::gl::GlslProgRef		mFloorVboShader;
	ci::gl::Texture*		mAlpha;
	ci::gl::Texture*		mBlur;
	ci::gl::Texture*		mParticle;

	ci::TriMesh			mFloorMesh;
	ci::gl::VboMesh			mFloorVbo;
	
	ci::Colorf*			mExtraVboData;

	//animation
	std::vector<ci::Vec3f>		mFloorVertices;
	std::vector<ci::Anim<ci::Vec3f>>	mAnimatingVertices;
	std::vector<int>		mUsedIndices;
	std::vector<int>		mJustAdded;
	
	int				mCenterPointIndex;
	int				mGridWidth;
	int				mGridHeight;
	int				mNumRecursive, mMaxRecursive;
	ci::Vec3f			mCenterPoint;
	int				mMaxDistance;
		
    ci::gl::Fbo*			mFbo1;
	ci::gl::Fbo*			mFbo2;

	//testing
	int				mPointCount;
	int				mMaxPoints;

	//particles 
	FloorParticlesRef	mParticles;

	//light beams!
	std::vector<LightBeamRef>* mLightBeams;
	int getClosestVertexIndex(ci::Vec3f lightPoint);
	LightBeamRef getLightBeam();

public:
	HandbookFloorView(Assets* assets);
	~HandbookFloorView(void);

	virtual void update(float dt);
	virtual void prerender();
	virtual void render();
	virtual void renderShaders();
	virtual void draw();
	virtual void drawQuad();
	
	void pingVerts();

	ci::Anim<ci::Vec3f> floorRotation;
	ci::Anim<ci::Vec3f> floorOffset;

	FloorParticlesRef getParticles() { return mParticles; };
};

typedef boost::shared_ptr<HandbookFloorView> HandbookFloorRef;