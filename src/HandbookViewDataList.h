#pragma once
#include "DataViewDataList.h"
#include "HandbookView3dObject.h"
#include "HandbookViewMovie.h"
#include "HandbookViewDocument.h"

class HandbookViewDataList :
	public DataViewDataList
{	
	typedef DataViewListIcon::DataViewListIconRef IconRef;

	typedef DataView3dObject::DataView3dObjectRef Obj3dRef;
	typedef DataViewDocument::DataViewDocumentRef DocRef;
	typedef DataViewMovie::DataViewMovieRef MovieRef;
	typedef DataViewAudio::DataViewAudioRef AudioRef;

protected: 
	virtual void buildContent(); 
	virtual void show(bool snap); 

public:
	HandbookViewDataList(DataNodeRef dn, Assets* assets);
	~HandbookViewDataList(void);
	
	virtual void onViewClose(); 
};

