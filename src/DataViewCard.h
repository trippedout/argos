#pragma once

#include "InputView.h"
#include "DataInnerViewBase.h"
#include "DataInnerViewSimple.h"
#include "DataNode.h"

class DataViewCard : public InputView
{
	typedef boost::shared_ptr<DataInnerViewBase>	InnerDisplayObjectRef;
	typedef boost::shared_ptr<DataInnerViewSimple>	InnerSimDisplayObjectRef;

protected:
	boost::shared_ptr<DataNode> dataNode;
	InnerDisplayObjectRef		mCardContent;
	InnerSimDisplayObjectRef	mCardContentSimple;
	ImageViewRef		mBarTop, mBarBottom;
	bool			active;
	ci::Anim<float>		transition;

public:
	DataViewCard( boost::shared_ptr<DataNode> data,  Assets* assets);
	~DataViewCard(void);

	boost::shared_ptr<DataNode> getDataNode(); 
	//
	
	void closeView(); 
	void addContent(); 
	void removeContent(); 

	virtual void prerender(); 
	virtual void reset(); 
	virtual void show(bool snap); 
	virtual void hide(bool snap); 

	virtual void onAnimateIn(); 
	virtual void onAnimateOut(); 

	virtual void handleInput(INPUT_TYPE type);
#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamepad);
#endif

	virtual void onContentInputBorder(InputView* view, InputView::InputState state);
};

