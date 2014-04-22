#pragma once

#include "cinder/Camera.h"
#include "VideoObject.h"
#include "VideoSnapshot.h"

#include "HostView.h"
#include "BugView.h"
#include "HandbookView.h"
#include "DataView.h"
#include "LocationView.h"

//#include "AnalyzeView.h"

#include "EngageView.h"
#include "PlayView.h"
#include "EndView.h"
#include "PrefsView.h"
#include "PauseView.h"
#include "StartView.h"
#include "Overlay.h"

#include "Settings.h"

#include "InputController.h"
#include "BackgroundController.h"
#include "BeaconView.h"
#include "SoundController.h"

#include "InputTypes.h"

class ViewController
{
protected: 
    ci::Vec3f camPos, camTarget, camUp;
	
	Assets* assets;
	
	InputController*		inputController;
	BackgroundController*	backgroundController; 
	
	BugView* bug; 
	VideoViewRef video;
	VideoSnapshotRef videoSnapshot; 

	Settings::ViewID current;
	//Settings::ViewID previous;
    std::vector<Settings::ViewID> backStack;

	std::map< Settings::ViewID, InputViewRef > views; 

	InputViewRef		mCurrentView;
	InputViewRef		mNextView;

	// timer for changing views
	// disbales 'handleInput' for 0.25 seconds everytime 'changeView' is called. 
	float viewChangeDelay;
	//very small delay (0.05 seconds) between recieving inputs. prevents spamming. 
	float inputDelay; 

	bool showBackground;
	bool firstRun;

	//TODO handle overlay views. 
	// progress, and beacon shit. 
	void changeView(Settings::ViewID id, bool fromBack = false); 
	void addView(Settings::ViewID id, InputViewRef view);
	void handleInput(INPUT_TYPE type);

public:
	ViewController(Assets* ass, InputController* ic);
	
	void render(int screenWidth, int screenHeight);
	void update(float dt);

	void dPad(ci::Vec2i dir); 
	void selectOld(); 
	void back(); 
	void showStart(); 
	void showPause(); 
	void showHandbook(); 
	void showBeacon(); 
	void nextScene(); 
	void prevScene(); 

	void onViewChangeSelection(InputView* view, Settings::ViewID id);

	void handleViewAnimationState(View* view, View::AnimState state);

	InputViewRef getCurrent();
	
	~ViewController(void);
};

