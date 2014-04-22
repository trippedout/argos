#pragma once

#include "InputView.h"
#include "VideoObject.h"
#include "cinder/qtime/QuickTime.h"

class DataViewAudio : public InputView
{
public:
	typedef boost::shared_ptr<DataViewAudio> DataViewAudioRef;

	DataViewAudio(DataNodeRef dn, std::string assetID, std::string assetPath, Assets* assets);
	~DataViewAudio(void);

	virtual void handleInput(INPUT_TYPE type);
	virtual void update(float dt); 
	virtual void disposeTempAssets(); 

	virtual void onAnimateOut(); 


protected: 
	ImageViewRef thumbnail;
	
	VideoViewRef video; 

	boost::shared_ptr<DataNode> dataNode; 
	bool playing; 

	std::string assetPath; 

	void exitVideo(); 
};

