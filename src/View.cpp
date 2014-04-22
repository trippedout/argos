#include "View.h"

using namespace boost::signals2;

View::View(Assets* assets) : DisplayObjectContainer()
{
	//set assets pointer
	mAssets = assets;
	
}

View::~View(void)
{

}

void View::update(float dt)
{
	DisplayObjectContainer::update(dt);

	//override me
}

void View::render()
{
	DisplayObjectContainer::render();
}

void View::prerender()
{
	// override this.
	// renders outside of the main update loop
	// use this for FBO rendering etc. 
}

void View::draw()
{
	DisplayObjectContainer::draw();

	//override me!
}	
void View::show()
{
	//override me!
}

void View::hide()
{
	//override me!
}
void View::addViewStateSignal(const StateSignal::slot_type& slot)
{
	connection c = mAnimStateSignal.connect(slot);
}

void View::removeViewStateSignal()
{
	//mAnimStateSignal.disconnect(slot);
	mAnimStateSignal.disconnect_all_slots();
}

//onAnimateIn() - override this function if you want to have your view animate in at all,
//and when the animation is COMPLETE, don't forget to either call this as a super or
//call onAnimateInComplete() directly so the state can be updated and dispatched
void View::onAnimateIn()
{
	mAnimStateSignal(this, AnimState::ANIMATING_IN);

	this->onAnimateInComplete();
}

//onAnimateInComplete() - this function will automatically 
void View::onAnimateInComplete()
{
	mAnimStateSignal(this, AnimState::ANIMATE_IN_COMPLETE);
}

//onAnimateOut() - override this function if you want to have your view animate out at all,
//and when the animation is COMPLETE, don't forget to either call this as a super or
//call onAnimateOutComplete() directly so the state can be updated and dispatched
void View::onAnimateOut()
{
	mAnimStateSignal(this, AnimState::ANIMATING_OUT);

	this->onAnimateOutComplete();
}

void View::onAnimateOutComplete()
{
	mAnimStateSignal(this, AnimState::ANIMATE_OUT_COMPLETE);
}