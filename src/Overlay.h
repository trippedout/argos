#pragma once
#include "OverlayView.h"
#include "Assets.h"

class Overlay
{

public:
	static Overlay& getInstance()
	{
		static Overlay instance;
		return instance;
	}

	void initView(Assets* assets); 
	OverlayView* getView(); 

	void setFullScreen(bool val); 
	bool getFullScreen(); 

	void setActive(bool val); 
	bool getActive(); 

	Overlay(void);
	~Overlay(void);

protected: 
	OverlayView* view; 
	bool fullScreen; 
	bool active; 

};

