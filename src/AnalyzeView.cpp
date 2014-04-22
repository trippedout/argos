#include "AnalyzeView.h"


AnalyzeView::AnalyzeView(Assets* assets) : InputView(assets)
{
	setPosition( Vec3f(490, 360, 0) ); 
	
	image1 = new ARGOSButtonView(assets, "IMAGE1");
	image2 =  new ARGOSButtonView(assets, "IMAGE2");

	image1->setPosition(Vec3f(0,0,0)); 
	image2->setPosition(Vec3f(512,0,0)); 

	addChild(DisplayObjectRef(image1)); 
	addChild(DisplayObjectRef(image2)); 
}

AnalyzeView::~AnalyzeView(void)
{
}

void AnalyzeView::draw()
{
	InputView::draw();

	image1->render(); 
	image2->render();
}

void AnalyzeView::handleInput(INPUT_TYPE type)
{

}
