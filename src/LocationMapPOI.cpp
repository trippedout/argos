#include "LocationMapPOI.h"

using namespace ci;
using namespace ci::app;

LocationMapPOI::LocationMapPOI(Assets* assets, std::string title) : View(assets)
{
	mLocation	=	TextViewRef( new TextView(title, assets->getFontByID("location_map_area"), ci::ColorA(1,1,1,1) ) );
	mLocation->setPosition( ci::Vec3f( mLocation->getSize()->value().x * -0.5f, 0.f, 0.f ) );
	this->addChild(mLocation);

	mArrow		=	ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "scenes/location/small_arrow.png" ) ) ) ) );
	mArrow->setPosition( ci::Vec3f( mArrow->getSize()->value().x * -0.5f, 30.f, 0.f ) );
	this->addChild( mArrow );
}


LocationMapPOI::~LocationMapPOI(void)
{
	mLocation.reset();
	mArrow.reset();
}

//void LocationMapPOI::render(ci::Vec3f camPos, ci::Vec3f lookAt)
//{
//	/*float deg = 57.2957795;
//	float scale, a, z, l, deltaX, deltaY, deltaZ, angleZ, hyp, angleY; 
//	ci::Vec3f pos;
//	
//	deltaX = lookAt.x - camPos.x;
//	deltaY = lookAt.y - camPos.y;
//	deltaZ = lookAt.z - camPos.z;
//	angleZ   = atan2( deltaY,deltaX ); 
//	hyp = sqrt( pow( deltaX, 2 ) + pow( deltaY, 2 ) ); 
//	angleY   = atan2( hyp, deltaZ );
//
//	this->setRotation(ci::Vec3f(-180, angleY, 0) );*/
//
//	View::render();
//}
