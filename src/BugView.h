#pragma once
#include "cinder/gl/Texture.h"
#include "View.h"
#include "ImageView.h"
#include "VideoObject.h"
#include "Bug.h"
#include "Settings.h"
#include "SoundController.h"

#include "DataController.h"

class BugView :	public View
{

protected:
	boost::shared_ptr< Bug > bug; 
	DataController* dataController;

	VideoViewRef video;
	Settings::ViewID currentView; 
	float checkDataTimer, checkDataTimerMax; 
	SceneRef previousScene; 

	void resetOniPulses(); 

public:
	BugView(Assets* assets, VideoViewRef vid);
	~BugView(void);

	virtual void update(float dt); 
	void changeView(Settings::ViewID id);

	void idle(); 
	void data(); 
	void progress(); 

	void voiceOn(); 
	void voiceOff(); 
};

