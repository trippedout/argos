#pragma once

#include "DataViewContentBase.h"
#include "DataView3dObject.h"
#include "DataViewDocument.h"
#include "DataViewMovie.h"
#include "DataViewAudio.h"
#include "DataViewListIcon.h"
#include "Overlay.h"

#include "DataNodeDataItem.h"

class DataViewDataList : public DataViewContentBase
{
	typedef DataViewListIcon::DataViewListIconRef IconRef;

	typedef DataView3dObject::DataView3dObjectRef Obj3dRef;
	typedef DataViewDocument::DataViewDocumentRef DocRef;
	typedef DataViewMovie::DataViewMovieRef MovieRef;
	typedef DataViewAudio::DataViewAudioRef AudioRef;

protected:
	DataNodeRef							dataNode; 
	DisplayObjectContainerRef			mContentHolder;
	InputViewRef						mCurrentContent;

	std::vector<ButtonViewRef>				mIconList;
	std::vector<InputViewRef>				mViewList;

	std::map<ButtonViewRef, InputViewRef>	mContentMap;


public:
	DataViewDataList(Assets* assets);
	DataViewDataList(DataNodeRef dn, Assets* assets);
	~DataViewDataList(void);

	virtual void onAnimateIn(); 
	virtual void prerender(); 
	virtual void moveSelection(ci::Vec2i dir);
	virtual void handleInput(INPUT_TYPE type);
#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamePad);
#endif
	virtual void disposeTempAssets();

	virtual void buildContent(); 

	virtual void show(bool snap); 
	virtual void hide(bool snap); 

	virtual void onViewOpen(); 
	virtual void onViewClose(); 
};

