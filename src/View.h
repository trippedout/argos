#pragma once

#include "DisplayObjectContainer.h"
#include "Assets.h"
#include "Settings.h"

#include <boost/signals2.hpp>
#include <boost/bind.hpp>

//using namespace boost::signals2;

typedef boost::shared_ptr<class View> ViewRef;

class View : public DisplayObjectContainer
{

public:
	enum AnimState { ANIMATING_IN, ANIMATE_IN_COMPLETE, ANIMATING_OUT, ANIMATE_OUT_COMPLETE };
	
	typedef			boost::signals2::signal<void (View*, View::AnimState)> StateSignal;

	View(Assets* assets);
	virtual ~View(void);

	virtual void update(float dt); 
	virtual void render(); 
	virtual void prerender(); 
	virtual void draw();

	//basics
	virtual void show(); 
	virtual void hide(); 

	//BUILD ON IT
	virtual void addViewStateSignal(const StateSignal::slot_type& slot);
	virtual void removeViewStateSignal();

	virtual void onAnimateIn();
	virtual void onAnimateInComplete();

	virtual void onAnimateOut();
	virtual void onAnimateOutComplete();

protected: 
	Assets*					mAssets;
	StateSignal				mAnimStateSignal;
};

