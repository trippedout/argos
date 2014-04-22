#include "BackgroundLower.h"

using namespace ci;

BackgroundLower::BackgroundLower(Assets* assets) : 
	BackgroundBase(assets),
	mDataFloor( DataFloorViewRef( new DataFloorView(assets) ) ),
	mFloatingParticles( LowerParticlesRef( new BackgroundLowerParticles(assets) ) )
{
	//setup the rest
	mBeam = ImageViewRef( new ImageView( assets->getTextureByID(assets->getId(Assets::FileID::LOWER_BEAM))));
	this->addChild(mBeam);

	//for manip of particles
	//mNoise = ci::gl::Texture( loadImage( loadAsset( "bgs/noise.png" ) ) );
	mSmallParticles = ParticleSystemRef( new BackgroundSmallParticleSystem(assets, 3200) );
	
	//advance particles
	for(int i = 0; i < 500; ++i)
	{
		this->update(0.f);
	}
}


BackgroundLower::~BackgroundLower(void)
{
	mDataFloor.reset();
}

void BackgroundLower::onAnimateIn()
{
	mDataFloor->animateIn();
	onAnimateInComplete();
}

void BackgroundLower::onAnimateOut()
{
	mDataFloor->animateOut();
	onAnimateOutComplete();
}

void BackgroundLower::update(float dt)
{	
	mDataFloor->update(dt);
	mSmallParticles->update();
	mFloatingParticles->update();
}


void BackgroundLower::prerender()
{
	mDataFloor->prerender();
}

void BackgroundLower::render()
{
	//BLEND SHIT
	gl::enableAdditiveBlending();

	glPushMatrix();
	{
		mBeam->render();
		mSmallParticles->render();
	
		mDataFloor->render();

		mFloatingParticles->render();
	}
	glPopMatrix();

	gl::enableAlphaBlending();
}


void BackgroundLower::handleInputFromView()
{
	//eventually do cool shit
}
