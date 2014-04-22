#include "BackgroundMainGlow.h"

using namespace ci::app;

BackgroundMainGlow::BackgroundMainGlow(Assets* assets) : View(assets)
{
//	const float circleAlpha = 0.45f;
	const int edgeOffset = 25;
	const int w = getWindowWidth();
	const int h = getWindowHeight();

	mGlow = ImageViewRef( new ImageView( assets->getTextureByID( "main_bg_glow" ) ) );
	//mGlow->setCenter(ci::Vec3f(1,.5,0));
	//mGlow->setPosition(ci::Vec3f(w, h/2,0));
	//mGlow->setAdditiveBlending(false);
	this->addChild(mGlow);

	mGlyphs = GlyphsRef( new BackgroundMainGlyphs(assets) );
	mGlyphs->setPosition(ci::Vec3f((float)w, (float)h/2.0f - (float)edgeOffset,0));
	this->addChild(mGlyphs);

}

BackgroundMainGlow::~BackgroundMainGlow(void)
{
	mGlow.reset();
	mGlyphs.reset();
}

void BackgroundMainGlow::update(float dt)
{

	mGlyphs->update(dt);
}

void BackgroundMainGlow::render()
{
	View::render();
}