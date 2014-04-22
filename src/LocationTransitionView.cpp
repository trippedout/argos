#include "LocationTransitionView.h"

using namespace ci;
using namespace ci::app;

LocationTransitionView::LocationTransitionView(Assets* assets) : 
	View(assets),
	mFirstRun(true),
	mCurrentTransition(NULL),
	mFrameCount(0),
	mFrameSwap(34),
	mHalfDuration(0.f),
	mIsHalfway(false),
	mOrbitalToAerial(ScrubVideoRef( new ScrubVideoObject(assets->getVideoByID("loc_orbital_aerial")))),
	mAerialToLocal(ScrubVideoRef( new ScrubVideoObject(assets->getVideoByID("loc_aerial_local"))))
	//mHandbookToOrbital(ScrubVideoRef( new ScrubVideoObject(assets->getVideoByID("loc_hb_orbital"))))
{
	mOrbitalToAerial->setScale(ci::Vec3f(2.f,2.f,2.f));	
	mAerialToLocal->setScale(ci::Vec3f(2.f,2.f,2.f));	
	//mHandbookToOrbital->setScale(ci::Vec3f(2.f,2.f,2.f));	
}

LocationTransitionView::~LocationTransitionView(void)
{
	mOrbitalToAerial->reset();
	mOrbitalToAerial.reset();
	
	mAerialToLocal->reset();
	mAerialToLocal.reset();

	//mHandbookToOrbital->reset();
	//mHandbookToOrbital.reset();
}

void LocationTransitionView::onAnimateIn()
{
	if(mFirstRun)
	{
		mFirstRun = false;
	}

	View::onAnimateInComplete();
}

void LocationTransitionView::startTransition(TransitionType type)
{
	bool playReverse = false;
	bool skipToHalfway = false;

	switch(type)
	{
		
	//animate in from another view! 
	case ORBITAL:
		mCurrentTransition = mOrbitalToAerial;
		skipToHalfway = true; 
		playReverse = true;
		break;

	case AERIAL:
		mCurrentTransition = mOrbitalToAerial;
		skipToHalfway = true; 
		break;

	case LOCAL:
		mCurrentTransition = mAerialToLocal;
		skipToHalfway = true; 
		break;

	//normal transitions
	case ORB_AERIAL:
		mCurrentTransition = mOrbitalToAerial;
		break;

	case AERIAL_ORB:
		mCurrentTransition = mOrbitalToAerial;
		playReverse = true;
		break;

	case AERIAL_LOCAL:
		mCurrentTransition = mAerialToLocal;
		break;

	case LOCAL_AERIAL:
		mCurrentTransition = mAerialToLocal; //mAerialToLocal
		playReverse = true;
		break;

	default:
		mCurrentTransition = mOrbitalToAerial;
		break;
	}

	if(mCurrentTransition)
	{
		this->addChild(mCurrentTransition);
		
		mHalfDuration = mCurrentTransition->getDuration() * .5f;
		mIsHalfway = false;

		if(playReverse)
		{
			mCurrentTransition->setSpeed(-1.f);
			mCurrentTransition->setTime(mCurrentTransition->getDuration());
		}
		else
		{
			mCurrentTransition->setSpeed(1.f);
			mCurrentTransition->setTime(0.f);
		}

		if(skipToHalfway){ 
			
			ci::app::console() << "LocationTransitionView::startTransition() mIsHalfway: true" << std::endl;
			mIsHalfway = true;
			mCurrentTransition->setTime(mHalfDuration);
			mTransitionSignal(TransitionEvent::TRANNY_SWAP);
		}

		mCurrentTransition->play();
	}

	//first tranny done
	mFirstRun = false;
}

void LocationTransitionView::update(float dt)
{
	View::update(dt);

	if(mCurrentTransition)
	{
		//ci::app::console() << "LocationTransitionView::update() has transition: " << mFrameCount << " swap at: " << mFrameSwap << std::endl;

		//check if its halfway playing forwards and backwards
		if(!mIsHalfway)
		{
			if(mCurrentTransition->getSpeed() > 0.f)
			{
				if( mCurrentTransition->getTime() >= mHalfDuration )
				{
					mIsHalfway = true;
					mTransitionSignal(TransitionEvent::TRANNY_SWAP);
				}
			}
			else
			{
				if( mCurrentTransition->getTime() <= mHalfDuration )
				{
					mIsHalfway = true;
					mTransitionSignal(TransitionEvent::TRANNY_SWAP);
				}
			}
		}

		//kill it when its done
		if(mCurrentTransition->isFinished())
		{
			if(this->contains(mCurrentTransition))
			{
				ci::app::console() << "LocationTransitionView::update() transition complete" << std::endl;
				//mCurrentTransition->stop();
				this->removeChild(mCurrentTransition);
				mCurrentTransition = NULL;

				mTransitionSignal(TransitionEvent::TRANNY_END);
			}
		}
	}
}

void LocationTransitionView::render()
{
	gl::enableAdditiveBlending();
		View::render();
	gl::enableAlphaBlending();
}