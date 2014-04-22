#pragma once
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "InputView.h"
#include "ARGOSButtonView.h"

class AnalyzeView :
	public InputView
{

protected:
	ARGOSButtonView* image1;
	ARGOSButtonView* image2;

public:
	AnalyzeView(Assets* assets);
	
	void draw(); 
	
	virtual void	handleInput(INPUT_TYPE type);

	~AnalyzeView(void);
};

