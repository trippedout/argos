#pragma once

#include "DisplayObject.h"
#include "ImageView.h"
#include "Assets.h"
#include "SoundController.h"
#include "ScrubVideoObject.h"
#include "SphereConstellation.h"

#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"
#include "cinder/Rand.h"

typedef boost::shared_ptr<class SphereView> SphereViewRef;

class SphereView : public DisplayObject
{	
protected: 
	
	ScrubVideoRef innerSphere; 
	ci::Vec3f sphereOffset;

	ci::Anim<ci::Vec3f> sphereRotation;
	
	ci::Anim<float> cAlpha1;
	ci::Anim<float> cSolidAlpha1;
	ci::Anim<float> cBorderAlpha1;

	ci::Anim<float> cAlpha2;
	ci::Anim<float> cSolidAlpha2;
	ci::Anim<float> cBorderAlpha2;
	ci::Anim<float> mSphereAlpha;

    ci::gl::Fbo*	fbo;
	ci::gl::VboMesh* vbo; 
	ci::gl::GlslProgRef bloomShader;
	ci::gl::GlslProgRef centerShader;
	ci::gl::GlslProgRef ringShader;
	ci::gl::Texture* centerTex;
	ci::gl::Texture* ringTex;
	
	bool waitingToSpin, idle; 
	float rotateDelay; 
	int frame, prevFrame, totalFrames, currentConst, prevConst, nextConst; 

	SphereConstellationRef mCurrentConst, mNextConst, mPrevConst;

	void changeConstellation(); 

public:
	SphereView(Assets* assets);
	~SphereView(void);

	std::vector<SphereConstellationRef>* constellations;

	void drawQuad(); 
	virtual void prerender(); 
	virtual void render(); 
	virtual void draw(); 
	virtual void update(float dt);

	void swapCenterShader(ci::gl::GlslProgRef shader);
	void swapBlurShader(ci::gl::GlslProgRef shader);
	void swapRingShader(ci::gl::GlslProgRef shader); 

	int getCurrentConstellation(){ return currentConst; }; 
	void setConstellation(int val);

	ci::Anim<ci::Vec3f>* getSphereRotation(){return &sphereRotation; };
	virtual void setSphereRotation( ci::Vec3f val ){ sphereRotation=val; }; 

	void onAnimateIn();
	void onAnimateOut();

	void setIdle(bool val){idle = val;}; 

protected:
	typedef boost::shared_ptr<class HBSphereParticle> SphereParticleRef;
	
	ci::gl::GlslProgRef					mParticleShader;
	ci::Vec4f*						mPositions;
	ci::Vec4f*						mParticleExtras;
	std::vector<SphereParticleRef>*	mParticles;
	int							mNumParticles;

	float						mSpinSet1Speed, mSpinSet2Speed, mSpinSet3Speed;
	float						mSet1RotVal, mSet2RotVal, mSet3RotVal;

	ci::Anim<float>					mParticleYOffset;
	ci::Anim<ci::Vec3f>					mParticlesRotation;
	ci::Anim<float>					mSet1Rot,mSet2Rot,mSet3Rot;
	ci::Anim<float>					mParticleAlpha;
	float						mParticleDrawDistance;
	
	void drawParticles();
	void onSwitchParticleDirection();
	void onAnimateParticlesOutComplete();
	void onAnimateParticlesInComplete();
	void onAnimateParticlesInUpdate();

public:
	void createShader();
	void animateParticlesIn();
	void animateParticlesOut();
};


class HBSphereParticle
{
	ci::Vec4f mStartingPosition, mPosition, mVelocity;
	float mSeedUpdateVal, mPosUpdateVal;
public:
	HBSphereParticle(ci::Vec4f startingPoint);
	~HBSphereParticle(void);
	
	void reset();
	void update();

	void setVelocity(ci::Vec4f velocity) {	mVelocity = velocity; };
	void setPosUpdateVal(float updateVal) { mPosUpdateVal = updateVal; };

	ci::Vec4f getPosition() { return mPosition; };
};

                                        