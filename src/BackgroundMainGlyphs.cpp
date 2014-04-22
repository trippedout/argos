#include "BackgroundMainGlyphs.h"

using namespace ci;
using namespace ci::app;

BackgroundMainGlyphs::BackgroundMainGlyphs(Assets* assets) : 
	View(assets),
	mShader( mAssets->getShaderByID("bloom")),
	mContainer(DisplayObjectContainerRef( new DisplayObjectContainer ) )
{
	const float alpha = 0.55f;
	const int edgeOffset = 25;
	
	mCircle = ImageViewRef( new ImageView( assets->getTextureByID( "main_bg_circle" ) ) );
	mCircle->setCenter(ci::Vec3f(0.5f,0.5f,0));
	mCircle->setScale(ci::Vec3f(0.5f,0.5f,0.5f));
	mCircle->setAlpha(alpha);	
	mContainer->addChild(mCircle);

	float deg = 360.0 / (float)numGlyphs;
	ci::Vec3f pos;
	ImageViewRef glyph;
	std::string id;

	for(int i = 1; i <= numGlyphs; ++i)
	{
		id = "main_bg_glyph";

		if(i <= 8 ) 
			id.append(toString(i));
		else
			id.append(toString(i-8));


		pos.x = rad * cos(toRadians(i * deg));
		pos.y = rad * sin(toRadians(i * deg));

		glyph = ImageViewRef( new ImageView( assets->getTextureByID(id) ) );
		glyph->setCenter(ci::Vec3f(0.5f,0.5f,0));
		glyph->setScale(ci::Vec3f(.35f, .35f, .35f));
		glyph->setAlpha(alpha + .1f);
		glyph->setPosition(pos);
		
		mGlyphs.push_back(glyph);
		mContainer->addChild(glyph);
	}

	mContainer->setCenter(ci::Vec3f(0.5f,0.5f,0));
	this->addChild( mContainer);

	mOuterCircle = ImageViewRef( new ImageView( assets->getTextureByID( "main_bg_outer_circle" ) ) );
	mOuterCircle->setCenter(ci::Vec3f(0.5f,0.5f,0));
	mOuterCircle->setScale(ci::Vec3f(0.6f,0.6f,0.6f));
	mOuterCircle->setAlpha(alpha);
	mOuterCircle->setPosition(ci::Vec3f(0.f, -(float)edgeOffset, 0.f ));
	this->addChild(mOuterCircle);
}


BackgroundMainGlyphs::~BackgroundMainGlyphs(void)
{
}

void BackgroundMainGlyphs::update(float dt)
{
	View::update(dt);
	
	float rx = (sin(getElapsedFrames() * .003f) ) * 8.0f;
	float ry = (sin(getElapsedFrames() * .001f) ) * 4.0f;
	mContainer->getRotation()->value().x = 7.0f + rx;
	mContainer->getRotation()->value().y = -12.0f + ry;
	mContainer->getRotation()->value().z += 0.06f;	

	//ci::app::console() << "BackgroundMainGlyphs::update() rotation: " << mContainer->getRotation()->value() << std::endl;

	ci::Vec3f* rot = &mContainer->getRotation()->value();

	for(int i = 0; i < numGlyphs; ++i)
	{
		ImageViewRef glyph = mGlyphs[i];
		glyph->setRotation(*rot * -1);
	}

	mOuterCircle->getRotation()->value().y += 0.1f;
}

void BackgroundMainGlyphs::render()
{
	//mShader->bind();
	gl::enableAdditiveBlending();
	View::render();
	gl::enableAlphaBlending();
	//mShader->unbind();
}