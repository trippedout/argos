#include "LightBeam.h"

using namespace ci;
using namespace ci::app;

/*
LightBeams take a pointer to a vector of pointers to Vec3f's. This is so you can independently animate
your vertices/points wherever you want and the lightbeam will stick to those points
*/
LightBeam::LightBeam(std::vector<ci::Vec3f*>* beamPoints) :
	mIsAnimating(false),
	mColor(ci::Vec4f(1.f,1.f,1.f,1.f)), mLineWidth(1.5f),
	mBeamPoints(beamPoints), mNumPoints(int(beamPoints->size())), mMaxPoint(0), mLength(-1.f), mPointLength(0.f), mPercentAlongLine(0.f)
{
	mPointLength = 1.f / mNumPoints;
}

LightBeam::LightBeam(std::vector<ci::Vec3f*>* beamPoints, ci::Vec4f color) :
	mIsAnimating(false),
	mColor(color), mLineWidth(1.5f),
	mBeamPoints(beamPoints), mNumPoints(int(beamPoints->size())), mMaxPoint(0), mLength(-1.f), mPointLength(0.f), mPercentAlongLine(0.f)
{
	mPointLength = 1.f / mNumPoints;
}

LightBeam::LightBeam(std::vector<ci::Vec3f*>* beamPoints, ci::Vec4f color, float lineWidth) :
	mIsAnimating(false),
	mColor(color), mLineWidth(lineWidth),
	mBeamPoints(beamPoints), mNumPoints(int(beamPoints->size())), mMaxPoint(0), mLength(-1.f), mPointLength(0.f), mPercentAlongLine(0.f)
{
	mPointLength = 1.f / mNumPoints;
}

LightBeam::~LightBeam(void)
{
}

void LightBeam::animateIn(float duration, float delay)
{
	mIsAnimating = true;

	timeline().apply( this->getLength(), -1.f, 0.f, duration )
		.delay(delay)
		.finishFn(boost::bind(&LightBeam::onAnimateComplete, this));
}

void LightBeam::animateInOut(float duration, float delay)
{
	mIsAnimating = true;

	timeline().apply( this->getLength(), -1.f, 0.f, duration )
		.delay(delay);
	timeline().appendTo( this->getLength(), 0.f, 1.f, duration )
		.finishFn(boost::bind(&LightBeam::onAnimateComplete, this));
}

void LightBeam::animateOut(float duration, float delay)
{
	mIsAnimating = true;

	timeline().apply( this->getLength(), 0.f, 1.f, duration )
		.delay(delay)
		.finishFn(boost::bind(&LightBeam::onAnimateComplete, this));
}

void LightBeam::onAnimateComplete()
{
	mIsAnimating = false;
}

void LightBeam::update(float dt)
{
	//if we're at starting point, dont do shit
	if(mLength.value() == -1.f) return;
	
	//if we've come full circle and drawn out, reset
	if(mLength.value() == 1.f) 
	{
		mLength = -1.f;
		return;
	}

	//now we're actually animating? get proper values here
	float position = 0.f;

	if( mLength.value() <= 0.0f )
		 position = map(mLength.value(), -1.f, 0.f, 0.f, 1.f);
	else
		position = map(mLength.value(), 0.f, 1.f, 1.f, 0.f);
	
	//ci::app::console() << "LightBeam::update() position: " << position << " along line: " << mPercentAlongLine << " max points: " << mMaxPoint << " total points: " << mNumPoints << std::endl;
	
	mMaxPoint = floorf(position / mPointLength);
	if(mMaxPoint == mNumPoints) mMaxPoint--;

	mPercentAlongLine = fmodf(position, mPointLength) / mPointLength;
	if(mPercentAlongLine > 1.f) mPercentAlongLine = 1.f;
}

void LightBeam::render()
{	
	//return;
	if(mLength.value() == -1.f) return;

	glPushMatrix();
	{
		gl::color(mColor.x, mColor.y, mColor.z, mColor.w * alpha->value());
		gl::lineWidth(mLineWidth);

		//if whole line, just draw that shit
		if( mLength.value() == 0.0f )
		{
			for( unsigned int i = 0; i < mNumPoints - 1; ++i )
			{
				gl::drawLine(*mBeamPoints->at(i), *mBeamPoints->at(i+1));
			}
			glPopMatrix();
			return;
		}

		//drawing in
		if( mLength.value() < 0.0f )
		{
			for( unsigned int i = 0; i < mMaxPoint; ++i )
			{
				if(i < mNumPoints - 1)
					gl::drawLine(*mBeamPoints->at(i), *mBeamPoints->at(i+1));
			}

			if( mMaxPoint < mNumPoints - 1 )
			{
				ci::Vec3f distance = *mBeamPoints->at(mMaxPoint+1) - *mBeamPoints->at(mMaxPoint);
				ci::Vec3f scaledDistance = distance * mPercentAlongLine;
				gl::drawLine(*mBeamPoints->at(mMaxPoint), *mBeamPoints->at(mMaxPoint) + scaledDistance);
			}
		}
		//drawing out
		else
		{	
			//ci::app::console() << "LightBeam::render() drawing out mMaxPoint: " << mMaxPoint << ", mNumPoints: " << mNumPoints << std::endl;
			for( unsigned int i = 0; i < mMaxPoint; ++i )
			{				
				if(i < mMaxPoint - 1)
					gl::drawLine(*mBeamPoints->at(mNumPoints - 1 - i), *mBeamPoints->at(mNumPoints - 1 - (i+1)));
			}
			
			if(mMaxPoint > 1)
			{
				ci::Vec3f distance = *mBeamPoints->at(mNumPoints - 1 - (mMaxPoint)) - *mBeamPoints->at(mNumPoints - mMaxPoint);
				ci::Vec3f scaledDistance = distance * mPercentAlongLine;
				gl::drawLine(*mBeamPoints->at(mNumPoints - mMaxPoint), *mBeamPoints->at(mNumPoints - mMaxPoint) + scaledDistance);
			}
		}
	}
	glPopMatrix();
}