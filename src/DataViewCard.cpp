#include "DataViewCard.h"

using namespace ci;
using namespace ci::app;

DataViewCard::DataViewCard(boost::shared_ptr<DataNode> data,  Assets* assets) : InputView(assets)
{
	transition = ci::Anim<float>(0);
	dataNode = data; 

	float offsetX = -1280/2.0f;

	mCardContentSimple	=	InnerSimDisplayObjectRef( new DataInnerViewSimple( data, assets) );
	//mCardContentSimple->setPosition(ci::Vec3f(offsetX,0,0));
	
	mCardContent		=	InnerDisplayObjectRef( new DataInnerViewBase(data, assets) );
	mCardContent->setPosition(ci::Vec3f(offsetX,0,0));
	
	mBarTop				=	ImageViewRef( new ImageView(assets->getTextureByID("card_bars")) );
	mBarTop->setAdditiveBlending(true);
	mBarTop->setPosition(ci::Vec3f(0,-35,0));
	mBarTop->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	mBarTop->project3D(true);
	mBarTop->setAlpha(0);

	mBarBottom			=	ImageViewRef( new ImageView(assets->getTextureByID("card_bars")) );
	mBarBottom->setAdditiveBlending(true);
	mBarBottom->setCenter(ci::Vec3f(0.5f,0.5f,0)); 
	mBarBottom->project3D(true);
	mBarBottom->setPosition(ci::Vec3f(0,675.0f,0));
	mBarBottom->setAlpha(0);

	
	addChild(mCardContentSimple);
	addChild(mBarTop); 
	addChild(mBarBottom); 

	mCardContent->getInputSignal()->connect(boost::bind(&DataViewCard::onContentInputBorder, this, boost::arg<1>::arg(), boost::arg<2>::arg()));

	active = true;
	hide(true); 
}

void DataViewCard::onContentInputBorder(InputView* view, InputView::InputState state)
{
	switch(state){
	case InputView::InputState::BORDER_LEFT:
		//ci::app::console() << "DataViewCard::onContentInputBorder BORDER_LEFT" << std::endl;
		mInputStateSignal(this, InputState::BORDER_LEFT); 
		break;

	case InputView::InputState::BORDER_RIGHT:
		//ci::app::console() << "DataViewCard::onContentInputBorder BORDER_RIGHT" << std::endl;
		mInputStateSignal(this, InputState::BORDER_RIGHT); 
		break;
	}
}

void DataViewCard::prerender()
{
	if(active) mCardContent->prerender();

	mCardContentSimple->prerender();
}

void DataViewCard::reset()
{
	mCardContent->reset();
}

void DataViewCard::closeView()
{
	//ci::app::console() <<"DataViewCard::closeView()" <<std::endl;
	//dispose of all temporary assets. 
	mCardContent->closeView(); 
}

void DataViewCard::show(bool snap)
{
	//if(active) return; 
	active = true; 
	
	//unblur shit 
	if(snap){
		addChild(mCardContent);
		mCardContent->show(snap); 
		mCardContentSimple->hide(snap); 
		timeline().apply(mBarTop->getAlpha(), 1.0f, 0);
		timeline().apply(mBarBottom->getAlpha(), 1.0f, 0);
		timeline().apply(mBarTop->getScale(), ci::Vec3f(1.0f,1.0f,0), 0);
		timeline().apply(mBarBottom->getScale(), ci::Vec3f(1.0f,1.0f,0), 0);
		timeline().apply(mBarTop->getPosition(), ci::Vec3f(0,-35,0), 0);
		timeline().apply(mBarBottom->getPosition(), ci::Vec3f(0,675.0f,0), 0);
		timeline().apply(mCardContentSimple->getBlur(), 0.0f, 0);
		timeline().apply(mCardContentSimple->getPosition(), ci::Vec3f(0,40,0), 0);
	}else{
		float t = 0.25f;
		
		timeline().apply(&transition, 1.0f, t).finishFn(std::bind(&DataViewCard::addContent,this));

		timeline().apply(mCardContentSimple->getBlur(), 0.0f, t, EaseOutQuint()).delay(t);
		timeline().apply(mCardContentSimple->getPosition(), ci::Vec3f(0,40,0), t, EaseInQuint()).delay(t);
	}

}

void DataViewCard::addContent()
{
	addChild(mCardContent);
	mCardContent->show(false); 
	mCardContentSimple->hide(false); 

	float t = 0.25f;
	//float offsetX = -1280/2.0f;
	timeline().apply(mBarTop->getAlpha(), 1.0f, t*0.5f, EaseOutQuint());
	timeline().apply(mBarBottom->getAlpha(), 1.0f, t*0.5f, EaseOutQuint());
	timeline().apply(mBarTop->getScale(), ci::Vec3f(1.0f,1.0f,0), t, EaseOutQuint()).delay(t*0.5f);
	timeline().apply(mBarBottom->getScale(), ci::Vec3f(1.0f,1.0f,0), t, EaseOutQuint()).delay(t*0.5f);
	timeline().apply(mBarTop->getPosition(), ci::Vec3f(0,-35,0), t*0.5f, EaseOutQuint());
	timeline().apply(mBarBottom->getPosition(), ci::Vec3f(0,675.0f,0), t*0.5f, EaseOutQuint());
}

void DataViewCard::hide(bool snap)
{
	//if(!active) return; 
	active = false; 
	
	//dispose of all temporary assets. 
	mCardContent->closeView(); 
	mCardContent->hide(snap);
	mCardContentSimple->show(snap); 
	
	float t = 0.25f;

	if(snap){
		t=0; 
		removeContent();
	}else{
		timeline().apply(&transition, 0.0f, t).finishFn(std::bind(&DataViewCard::removeContent,this));
	}


	timeline().apply(mBarTop->getAlpha(), 0.0f, t, EaseOutQuint());
	timeline().apply(mBarBottom->getAlpha(), 0.0f, t, EaseOutQuint());
	timeline().apply(mBarTop->getScale(), ci::Vec3f(0.4f,1.0f,0), t, EaseOutQuint());
	timeline().apply(mBarBottom->getScale(), ci::Vec3f(0.4f,1.0f,0), t, EaseOutQuint());
	timeline().apply(mBarTop->getPosition(), ci::Vec3f(0,40,0), t*0.5f, EaseOutQuint());
	timeline().apply(mBarBottom->getPosition(), ci::Vec3f(0,160.0f,0), t, EaseOutQuint());

	timeline().apply(mCardContentSimple->getPosition(), ci::Vec3f(0,0,0), t, EaseInOutQuint()).delay(t);
	timeline().apply(mCardContentSimple->getBlur(), 1.0f, t, EaseOutQuint()).delay(t);
}

void DataViewCard::removeContent()
{
	removeChild(mCardContent); 
}

void DataViewCard::onAnimateIn()
{
	if(active){
		float t = 1.0f;
		mBarTop->setAlpha(0);
		mBarBottom->setAlpha(0);
		timeline().apply(mBarTop->getAlpha(), 1.0f, t*0.5f, EaseOutQuint());
		timeline().apply(mBarBottom->getAlpha(), 1.0f, t*0.5f, EaseOutQuint());

		mCardContent->onAnimateIn(); 
	}else{
		//alpha in simple content view? 
		hide(true);
	}
}

void DataViewCard::onAnimateOut()
{
	if(active){
		mCardContent->onAnimateOut(); 
		timeline().apply(getAlpha(), 0.0f, 0.5f, EaseOutQuint());
		timeline().apply(mBarTop->getAlpha(), 0.0f, 0.5f, EaseOutQuint());
		timeline().apply(mBarBottom->getAlpha(), 0.0f, 0.5f, EaseOutQuint());
	}else{
		// dont need to do anything, dataFBO alphas out non selected items. 
	}
}

void DataViewCard::handleInput(INPUT_TYPE type)
{
	if(active) mCardContent->handleInput(type);
}

#ifdef _WIN32
void DataViewCard::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	if(active) mCardContent->handleGamepad(gamepad);
}
#endif

boost::shared_ptr<DataNode> DataViewCard::getDataNode()
{
	return dataNode; 
}

DataViewCard::~DataViewCard(void)
{

}