#pragma once

#include "InputView.h"
#include "DataNode.h"
#include "HandbookView3dObject.h"
#include "ScrubVideoObject.h"
#include "SphereView.h"
#include "ImageView.h"

#include "HandbookFloorView.h"
#include "HandbookColorBlurView.h"

typedef boost::shared_ptr<class HandbookBackgroundView> HandbookBackgroundViewRef;

class HandbookBackgroundView : public InputView
{
	typedef boost::shared_ptr<HandbookFloorView> HandbookFloorRef;
protected: 
	HandbookView3dObjectRef object3D;
	HandbookView3dObjectRef nextObject3D;

	ScrubVideoRef platform; 
	
	ImageViewRef bgBox; 
	SphereViewRef sphere;
	ImageViewRef sphereBG; 
	ImageViewRef platformBG;
	ImageViewRef beam;
	HandbookColorBlurViewRef colorBlur; 

	bool platformOpen, platformAdded; 
	
	HandbookFloorRef mFloor;

public:
	HandbookBackgroundView( Assets* assets );
	~HandbookBackgroundView(void);

	virtual void handleInput( INPUT_TYPE type );
#ifdef _WIN32
	virtual void handleGamepad( XINPUT_GAMEPAD gamepad );
#endif

	virtual void showPlatform();
	virtual void hidePlatform(); 
	virtual void setObject3D( HandbookView3dObjectRef val ); 
	virtual void removeObject3D(); 

	virtual void hideObject3D();
	virtual void showObject3D();
	virtual void setConstellation(int val); 

	virtual void update(float dt); 
	
	virtual void onAnimateIn(); 
	virtual void onAnimateOut(); 

	void showSphere(); 

	virtual void prerender();
	//virtual void render();
};