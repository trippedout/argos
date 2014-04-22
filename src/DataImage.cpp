#include "DataImage.h"

using namespace ci;
using namespace ci::app;

DataImage::DataImage(ci::gl::Texture* tex, Assets* assets):DisplayObjectContainer()
{
	dust = boost::shared_ptr<AngleShaderImageView>( new AngleShaderImageView(assets->getTextureByID("data_dust"), assets->getShaderByID("datadust")) ); 
	image = boost::shared_ptr<AngleShaderImageView>( new AngleShaderImageView(tex, assets->getShaderByID("datadusthead")) );  
	bg = ImageViewRef( new ImageView(assets->getTextureByID("hb_character") ) ); 
	bg->setAlpha(0.8f);

	dust->setAdditiveBlending(true);
	image->setAdditiveBlending(true);

	addChild(dust);
	addChild(bg); 
	addChild(image);
}

DataImage::DataImage(ci::gl::Texture* tex, ci::gl::GlslProgRef imageShader, Assets* assets):DisplayObjectContainer()
{
	//dust = boost::shared_ptr<AngleShaderImageView>( new AngleShaderImageView(assets->getTextureByID("data_dust"), assets->getShaderByID("datadust")) ); 
	
	image = boost::shared_ptr<AngleShaderImageView>( new AngleShaderImageView(tex, imageShader) );  
	image->setSpeed(3.0f); 
	
	//dust->setAdditiveBlending(true);
	image->setAdditiveBlending(true);

	//addChild(dust);
	addChild(image);
}

void DataImage::setImageAdditiveBlending(bool val)
{
	image->setAdditiveBlending(val);
}
void DataImage::showBG(bool val)
{
	if(bg){
		if(val){
			if(dust) addChild(dust);
			addChild(bg); 
			addChild(image);
		} 
		else {
			removeChild(bg); 
		}
	}
}
void DataImage::changeImage(ci::gl::Texture* tex)
{
	image->setTexture(tex); 
	image->setAlpha(0);
	timeline().apply(image->getAlpha(), 1.0f, 0.3f, EaseOutCubic());
	if(bg){
		bg->setAlpha(0);
		timeline().apply(bg->getAlpha(), 0.8f, 0.3f, EaseOutCubic());
	}
}

DataImage::~DataImage(void)
{
}
