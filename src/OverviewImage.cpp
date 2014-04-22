#include "OverviewImage.h"

using namespace ci;
using namespace ci::app;

OverviewImage::OverviewImage(Assets* assets) : View(assets)
{
	mGuide = ImageViewRef( new ImageView( new ci::gl::Texture( loadImage( loadAsset( "guides/data_overview_image.png")))) );
	this->addChild(mGuide);
}


OverviewImage::~OverviewImage(void)
{
}
