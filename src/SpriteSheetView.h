#pragma once
#include "ImageView.h"

typedef boost::shared_ptr<class SpriteSheetView> SpriteSheetViewRef;

class SpriteSheetView :
	public ImageView
{
protected:
	ci::Vec2i spriteSize; 
	int frame, totalFrames;  
	float frameRate, frameTime; 

public:
	SpriteSheetView(ci::gl::Texture* tex, ci::Vec2i spriteSize, int totalFrames, float framerate);
	~SpriteSheetView(void);

	virtual void update(float dt);
	virtual void drawQuad(); 
};