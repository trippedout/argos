#include "OverlayView.h"

OverlayView::OverlayView(Assets* assets) : InputView(assets)
{
	//addChild(ImageViewRef(new ImageView(assets->getTextureByID("fpo"))));

	toastView = ToastViewRef( new ToastView(assets) ); 
	addChild(toastView);
}

void OverlayView::toast(std::string copy)
{
	toastView->toast(copy); 
}

void OverlayView::addChild(DisplayObjectRef object)
{
	View::addChild(object);

	//make sure toastView is always on top
	if(object != toastView){
		addChild(toastView);
	}
}

void OverlayView::handleInput(INPUT_TYPE type)
{
	switch(type)
	{
	case BACK:
		//signal(this, Settings::ViewID::BACK);
		break;
	}
}

#ifdef _WIN32
void OverlayView::handleGamepad(XINPUT_GAMEPAD gamepad)
{

}
#endif
 
OverlayView::~OverlayView(void)
{

}