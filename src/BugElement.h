#pragma once

#include "DisplayObjectContainer.h"
#include "ImageView.h" 

typedef boost::shared_ptr<class BugElement> BugElementRef;

class BugElement :
	public DisplayObjectContainer
{
public:
	BugElement(ci::gl::Texture* tex, ci::gl::Texture* glowtex);
	~BugElement(void);

	void fancyPulse(float delay);
	void fancyGlowPulse(float delay);
	void fancyGlowPulseLoop(float delay);
	void selectGlowLoop(float delay);
	void fancyGlowPulse3X(float delay);

	void fancyGlowPulseLoop2(); 
	void selectGlowLoop2();

	void pulse();
	void select(); 
	void glow(); 
	void show(); 
	void hide(); 

	void setGlowColor(ci::Vec3f color); 

protected: 
	ImageViewRef baseImage; 
	ImageViewRef glowImage; 
};
