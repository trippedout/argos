#pragma once

#include "InputView.h"
#include "Overlay.h"
#include "VideoObject.h"
#include "cinder/qtime/QuickTime.h"
#include "SoundController.h"

class DataViewMovie : public InputView
{
public:
	typedef boost::shared_ptr<DataViewMovie> DataViewMovieRef;

	DataViewMovie(DataNodeRef dn, std::string assetID, std::string assetPath, Assets* assets);
	~DataViewMovie(void);

	virtual void handleInput(INPUT_TYPE type);
    
#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamepad);
#endif

	virtual void update(float dt);
	virtual void disposeTempAssets(); 
	
	virtual void onAnimateOut();

protected: 
	ImageViewRef thumbnail;
	ImageViewRef thumbBG; 
	
	VideoViewRef video; 

	boost::shared_ptr<DataNode> dataNode; 
	bool fullScreen; 
	bool playing; 

	std::string assetPath; 
	float inputDelay, frameSeekDelay, seekSpeed, rate;
	bool fastforwarding, rewinding; 

	void exitFullScreen(); 
};

