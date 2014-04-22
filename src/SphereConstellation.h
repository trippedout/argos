#pragma once

#include "DisplayObject.h"
#include "LightBeam.h"

#include "cinder/gl/Vbo.h"

typedef boost::shared_ptr<class SphereConstellation> SphereConstellationRef;

class SphereConstellation :	public DisplayObject
{
	typedef boost::signals2::signal<void()> AnimateOutCompleteSignal;
protected:
	LightBeamRef mLightBeam;
	ci::gl::VboMesh* mMesh;
	ci::TriMesh* mOutline;
	ci::Vec3f mRotation; 

	ci::Anim<float>		mInnerAlpha, mSolidALpha;
	ci::Anim<float>*	mSphereAlpha;

	AnimateOutCompleteSignal mAnimateOutCompleteSignal;

public:
	SphereConstellation(ci::gl::VboMesh* mesh, ci::TriMesh* outline, ci::Vec3f rotation);
	~SphereConstellation(void);

	void animateIn(ci::Anim<float>* sphereAlpha);
	void animateOut();

	void update(float dt);
	void render();

	ci::gl::VboMesh* getMesh() { return mMesh; };
	ci::TriMesh* getOutline() { return mOutline; };
	ci::Vec3f getRotation() { return mRotation; };

	AnimateOutCompleteSignal* getAnimateOutCompleteSignal() { return &mAnimateOutCompleteSignal; };
};

