#pragma once

#include "View.h"
#include "DataNode.h"
#include "ImageView.h"
#include "TextView.h"

class DataViewTitle : public View
{
protected:
	ImageViewRef	mIcon;
	TextViewRef		mTitle;
	TextViewRef		mSubtitle;

public:
	DataViewTitle(boost::shared_ptr<DataNode> data,  Assets* assets);
	~DataViewTitle(void);

	void project3D(bool val);
	void slideHack(float t); 
};

