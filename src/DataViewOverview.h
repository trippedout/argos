#pragma once

#include "DataViewContentBase.h"
#include "TextViewMultiline.h"
#include "DataImage.h"
#include "DataNode.h"
#include "BugElement.h"
#include "Overlay.h"

class DataViewOverview : public DataViewContentBase
{
	typedef boost::shared_ptr<TextViewMultiline> TvMultiDisplayObjectRef;
	typedef boost::shared_ptr<DataImage> OvImageViewRef;

protected:
	TvMultiDisplayObjectRef mText;
	OvImageViewRef mImage;
	BugElementRef arrow; 
	bool arrowLit; 
	DataNodeRef dataNode;

public:
	DataViewOverview(DataNodeRef dataNode, Assets* assets);
	~DataViewOverview(void);
#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamepad);
#endif
	virtual void handleInput(INPUT_TYPE type);
	void hideImage(); 

	virtual void show(bool snap); 
	virtual void hide(bool snap); 

	virtual void onViewOpen(); 
	virtual void onViewClose(); 
	
	virtual void checkOniUnlock(); 
};

