#pragma once

#include "View.h"
#include "ImageView.h"

#include "cinder/gl/GlslProg.h"

class BackgroundMainGlyphs : public View
{
	static const int numGlyphs = 14, rad = 150;
protected:
	ImageViewRef					mCircle;
	ImageViewRef					mOuterCircle;

	std::vector<ImageViewRef>			mGlyphs;

	DisplayObjectContainerRef		mContainer;

	ci::gl::GlslProgRef						mShader;

public:
	BackgroundMainGlyphs(Assets* assets);
	~BackgroundMainGlyphs(void);

	virtual void update(float dt);
	virtual void render();
};

