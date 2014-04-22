#include "LocationLocalView.h"

using namespace ci;
using namespace ci::app;

LocationLocalView::LocationLocalView(Assets* assets) : 
	LocationBaseView(assets),
	mBg( ImageViewRef( new ImageView( assets->getTextureByID("loc_local_bg") ) ) ),
	mControls( ImageViewRef( new ImageView( assets->getTextureByID("loc_local_controls") ) ) ),
	mRings( RingsRef( new LocationLocalBgRingsView(assets) ) ),
	mParticleSystem( ParticlesRef( new LocationLocalParticleSystem(assets) ) ),
	mUseMap(true)
{	
	mRings->setPosition(ci::Vec3f(getWindowWidth()/2, 40,0));
	mRings->setRotation(ci::Vec3f(-74,0,0));
	
	mControls->setPosition(ci::Vec3f(400,950,0));

	if(mUseMap)
	{
		mMap		=	MapRef( new LocationLocalViewMap(assets) );
		this->addChild(mMap);
	}
}

LocationLocalView::~LocationLocalView(void)
{
	mBg.reset();
	mRings.reset();

	if(mMap) mMap.reset();

	mParticleSystem.reset();
}

void LocationLocalView::setMapPosition(const std::string &mapId)
{
	ci::app::console() << "LocationLocalView::setMapPosition() " << mapId << std::endl;

	if(mMap) mMap->setMapPosition(mapId);
}

void LocationLocalView::onTransitionStart()
{
	LocationBaseView::onTransitionStart();
}

void LocationLocalView::onTransitionSwap(bool isNext)
{
	if(isNext)
	{
		
	}
	else
	{
		onTransitionEnd();
	}
}

void LocationLocalView::onTransitionEnd()
{
	LocationBaseView::onTransitionEnd();
	
}

void LocationLocalView::update(float dt)
{
	if(!mIsTransitioning) mParticleSystem->update();

	if(mMap)
	{
		mMap->update(dt);
		mRings->setCameraRotation(mMap->getCameraRotation());
	}
}

void LocationLocalView::render()
{	
	mBg->render();

	gl::enableAdditiveBlending();
		mRings->render();
	gl::enableAlphaBlending();

	mParticleSystem->render();
	
	if(mMap)mMap->render();

	mControls->render();
}

void LocationLocalView::handleInput(INPUT_TYPE type)
{
	if(mMap)mMap->handleInput(type);
}

#ifdef _WIN32
void LocationLocalView::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	if(mMap)mMap->handleGamepad(gamepad);
}
#endif