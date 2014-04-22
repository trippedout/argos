#pragma once

#include "InputView.h"
#include "DataImage.h"
#include "Overlay.h"

class DataViewDocument : public InputView
{
public:
	typedef boost::shared_ptr<DataViewDocument> DataViewDocumentRef;

	DataViewDocument(DataNodeRef dn, std::string assetID, std::string assetPath, Assets* assets);
	~DataViewDocument(void);

	virtual void handleInput(INPUT_TYPE type);
#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamepad);
#endif
	virtual void update(float dt);

	virtual void disposeTempAssets(); 

protected: 
	ImageViewRef thumbnail;
	ImageViewRef bg;
	ImageViewRef fullScreenBg;

	boost::shared_ptr<DataNode> dataNode; 
	ImageViewRef bigImage;
	ImageViewRef bgBox;

	bool active;
	float stick;
	std::string assetPath; 

};

