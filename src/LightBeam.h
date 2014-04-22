#pragma once

#include "DisplayObject.h"

typedef boost::shared_ptr<class LightBeam> LightBeamRef;

class LightBeam : public DisplayObject
{
protected:
	ci::Vec4f			mColor;
	float			mLineWidth;
	
	std::vector<ci::Vec3f*>* mBeamPoints;
	int				mNumPoints;
	int				mMaxPoint; // draw line up to this point when animating
	float			mPointLength;
	float			mPercentAlongLine;

	bool			mIsAnimating;

	ci::Anim<float>		mLength;

	void onAnimateComplete();	

	float map(float value, float start1, float stop1, float start2, float stop2) 
	{
		return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	}; 

public:
	void update(float dt);
	void render();

	ci::Anim<float>* getLength() { return &mLength; };

	void animateIn(float duration, float delay = 0.f);
	void animateInOut(float duration, float delay = 0.f);
	void animateOut(float duration, float delay = 0.f);

	bool getIsAnimating() { return mIsAnimating; };

	LightBeam(std::vector<ci::Vec3f*>* beamPoints);
	LightBeam(std::vector<ci::Vec3f*>* beamPoints, ci::Vec4f color);
	LightBeam(std::vector<ci::Vec3f*>* beamPoints, ci::Vec4f color, float lineWidth);
	~LightBeam(void);
};