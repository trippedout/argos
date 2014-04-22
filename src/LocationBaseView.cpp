#include "LocationBaseView.h"


LocationBaseView::LocationBaseView(Assets* assets) : 
	InputView(assets),
	mIsTransitioning(false)
{

}


LocationBaseView::~LocationBaseView(void)
{
}

void LocationBaseView::onTransitionStart()
{
	mIsTransitioning = true;
}

void LocationBaseView::onTransitionSwap(bool isNext)
{
}

void LocationBaseView::onTransitionEnd()
{
	mIsTransitioning = false;
}
