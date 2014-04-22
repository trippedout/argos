#pragma once

#include "View.h"
#include "ImageView.h"
#include "BackgroundMainGlyphs.h"


class BackgroundMainGlow : public View
{
	typedef boost::shared_ptr<BackgroundMainGlyphs> GlyphsRef;

protected:
	ImageViewRef		mGlow;
	GlyphsRef			mGlyphs;


public:
	BackgroundMainGlow(Assets* assets);
	~BackgroundMainGlow(void);


	virtual void update(float dt);
	virtual void render();
};

