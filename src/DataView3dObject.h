#pragma once

#include "InputView.h"
#include "RotationView.h"

class DataView3dObject : public InputView
{
protected: 
	boost::shared_ptr<RotationView> object;
	boost::shared_ptr<DataNode> dataNode;

	float stick, speed, delay;

public:
	typedef boost::shared_ptr<DataView3dObject> DataView3dObjectRef;
	
	DataView3dObject(Assets* assets);
	DataView3dObject(DataNodeRef dn, std::string assetID, Assets* assets);
	~DataView3dObject(void);

	virtual void prerender(); 
	virtual void update(float dt);
	virtual void handleInput(INPUT_TYPE type);
#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamepad);
#endif

	virtual void onAnimateIn();
	virtual void onAnimateOut();

	virtual void disposeTempAssets(); 

};

