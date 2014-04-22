#pragma once

#include "LocationBaseView.h"
#include "LocationLocalViewMap.h"
#include "LocationLocalBgRingsView.h"
#include "LocationLocalParticleSystem.h"

class LocationLocalView : public LocationBaseView
{
	typedef boost::shared_ptr<LocationLocalViewMap> MapRef;
	typedef boost::shared_ptr<LocationLocalBgRingsView> RingsRef;
	typedef boost::shared_ptr<LocationLocalParticleSystem> ParticlesRef;

protected:
	
	ImageViewRef				mBg;
	MapRef						mMap;
	RingsRef					mRings;

	ParticlesRef				mParticleSystem;

	ImageViewRef				mControls;

	bool	mUseMap;

public:
	LocationLocalView(Assets* assets);
	~LocationLocalView(void);
	
	void setMapPosition(const std::string &mapId);

	virtual void onTransitionStart();
	virtual void onTransitionSwap(bool isNext);
	virtual void onTransitionEnd();

	virtual void update(float dt);
	virtual void render();
	virtual void handleInput(INPUT_TYPE type);
#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamepad);
#endif
	
};

