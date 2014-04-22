#include "HandbookDataView.h"

using namespace ci;
using namespace ci::app;

HandbookDataView::HandbookDataView(HandbookBackgroundViewRef background, Assets* assets) : 
	InputView(assets), 
	mBackground(background),
	transition(ci::Anim<float>(0)),
	mRemoveItem(NULL),
	mCurrentItem(NULL)
{
	std::vector<DataNodeRef> nodes = *assets->getData()->getDataNodes(); 
	int size = nodes.size(); 

	for(int i=0; i<size; i++){
		mDataItems[ nodes[i] ] = HandbookDataItemRef( new HandbookDataItem(background, nodes[i], assets) ); 
		mDataItems[ nodes[i] ]->setPosition(ci::Vec3f(200.0f-15.0f,185.0f + 60,0)); 
	}
}

HandbookDataView::~HandbookDataView(void)
{

}

void HandbookDataView::setDataNode(DataNodeRef node)
{
	//if(!mDataItems[node]) mDataItems[ node ] = HandbookDataItemRef( new HandbookDataItem(node, mAssets) ); 
	mCurrentItem = mDataItems[node];
}

void HandbookDataView::onAnimateIn()
{
	if(mCurrentItem){
		addChild(mCurrentItem);
		mCurrentItem->onAnimateIn();

		if(mCurrentItem->getObject3D()){
			mBackground->setObject3D( mCurrentItem->getObject3D() );
		}

		mBackground->showPlatform(); 
	}
}

void HandbookDataView::onAnimateOut()
{
	if(mCurrentItem){
		timeline().apply(&transition, 1.0f, 0.5f).finishFn( std::bind(&HandbookDataView::removeItem,this) );
		mCurrentItem->onAnimateOut(); 
		
		mRemoveItem = mCurrentItem;
		mCurrentItem=NULL;

		mBackground->removeObject3D();
		mBackground->hidePlatform(); 
	}
}

void HandbookDataView::removeItem()
{
	if(mRemoveItem){
		removeChild(mRemoveItem);
		mRemoveItem = NULL;
	}
}

void HandbookDataView::prerender()
{
	if(mCurrentItem) mCurrentItem->prerender();
}

void HandbookDataView::handleInput(INPUT_TYPE type)
{
	if(mCurrentItem) mCurrentItem->handleInput(type);
}

#ifdef _WIN32
void HandbookDataView::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	if(mCurrentItem) mCurrentItem->handleGamepad(gamepad);
}
#endif
