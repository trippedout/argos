#pragma once

#include "cinder/gl/GlslProg.h"

#include "ImageView.h"

#include "LocationBaseView.h"
#include "LocationOrbitalOverlay.h"

#include "BackgroundMainGlyphs.h"
#include "BackgroundSmallParticleSystem.h"

class LocationOrbitalView :	public LocationBaseView
{
	typedef boost::shared_ptr<BackgroundMainGlyphs> GlyphRef;
	typedef boost::shared_ptr<LocationOrbitalOverlay> OverlayRef;
protected:
	SmallParticleSystemRef	mParticles;
	ImageViewRef			mPlanet;
	ImageViewRef			mLight;
	GlyphRef				mGlyphs;
	OverlayRef				mOverlay;
	ci::gl::Texture*				mNoiseTexture;
	ci::gl::GlslProgRef				mPlanetShader;
public:
	LocationOrbitalView(Assets* assets);
	~LocationOrbitalView(void);

	void setPlanet(std::string planetId);
	
	virtual void onTransitionStart();
	virtual void onTransitionSwap(bool isNext);
	virtual void onTransitionEnd();

	virtual void update(float dt);
	virtual void prerender();
	virtual void render();

	virtual void onAnimateIn();
	virtual void onAnimateOut();

	virtual void handleInput(INPUT_TYPE type);
#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamepad);
#endif
};

