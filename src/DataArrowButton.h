#pragma once
#include "DisplayObjectContainer.h"
#include "Assets.h"

#include "ImageView.h"

class DataArrowButton :
	public DisplayObjectContainer
{
protected: 
	ImageViewRef arrow; 
	ImageViewRef arrow2; 
	ImageViewRef arrow3; 
	
	ImageViewRef info; 
	
	bool active; 

public:
	DataArrowButton(Assets* assets, bool next);
	~DataArrowButton(void);

	void pulse(); 
	void show(); 
	void hide(); 
};

