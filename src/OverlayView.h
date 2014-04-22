#pragma once
#include "InputView.h"
#include "Assets.h"
#include "ToastView.h"

class OverlayView: public InputView
{

public:
	OverlayView(Assets* assets);
	~OverlayView(void);

	virtual void handleInput(INPUT_TYPE type);
    
#ifdef _WIN32
	virtual void handleGamepad(XINPUT_GAMEPAD gamepad);
#endif
    
	void toast(std::string copy); 

	virtual void addChild(DisplayObjectRef object); 
	
protected: 
	ToastViewRef toastView; 

};
