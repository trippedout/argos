#include "LocationLocalBgRingsView.h"

using namespace ci;
using namespace ci::app;

LocationLocalBgRingsView::LocationLocalBgRingsView(Assets* assets) : 
	View(assets),
	mRotation(0.f)
{
	const ci::Vec3f pos = ci::Vec3f(0,0,30);
	mNsew = ImageViewRef( new ImageView( assets->getTextureByID("loc_bg_nsew") ) );
	mNsew->setPosition(pos);
	mNsew->setRotation(ci::Vec3f(6,0,0));
	mNsew->setCenter(ci::Vec3f(0.5f, 0.5f,0));
	this->addChild(mNsew);

	mInnerCircle = ImageViewRef( new ImageView( assets->getTextureByID("loc_bg_inner_circle") ) );
	mInnerCircle->setPosition(pos);
	mNsew->setRotation(ci::Vec3f(6,0,0));
	mInnerCircle->setCenter(ci::Vec3f(0.5f, 0.5f,0));
	this->addChild(mInnerCircle);

	mInnerShapes = ImageViewRef( new ImageView( assets->getTextureByID("loc_bg_inner_shapes") ) );
	mInnerShapes->setCenter(ci::Vec3f(0.5f, 0.5f,0));
	this->addChild(mInnerShapes);

	mOuterCircle = ImageViewRef( new ImageView( assets->getTextureByID("loc_bg_outer_circle") ) );
	mOuterCircle->setAlpha(.55f);
	mOuterCircle->setCenter(ci::Vec3f(0.5f, 0.5f,0));
	this->addChild(mOuterCircle);

	mStaticCircles = ImageViewRef( new ImageView( assets->getTextureByID("loc_bg_static_circles") ) );
	mStaticCircles->setAlpha(.4f);
	mStaticCircles->setCenter(ci::Vec3f(0.5f, 0.5f,0));
	this->addChild(mStaticCircles);

	this->setCenter(ci::Vec3f(0.5f,0.5f,0));
}


LocationLocalBgRingsView::~LocationLocalBgRingsView(void)
{
	mNsew.reset();
	mInnerCircle.reset();
	mInnerShapes.reset();
	mOuterCircle.reset();
	mStaticCircles.reset();
}

void LocationLocalBgRingsView::update(float dt)
{
	//mNsew->getRotation()->value().z += 0.05;
}

void LocationLocalBgRingsView::setCameraRotation(float rotation)
{
	float deg = toDegrees(rotation);
	mNsew->setRotation(ci::Vec3f(0,0,deg));
	mInnerShapes->setRotation(ci::Vec3f(0,0,deg));
}