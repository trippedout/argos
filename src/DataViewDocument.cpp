#include "DataViewDocument.h"

using namespace ci::app;

DataViewDocument::DataViewDocument(boost::shared_ptr<DataNode> dn, std::string assetID, std::string path, Assets* assets) : InputView(assets)
{
	assetPath = path; 
	dataNode = dn; 
	thumbnail = ImageViewRef( new ImageView(assets->getTextureByID(assetID + "_small") ) );
	thumbnail->setPosition(ci::Vec3f(100.0f,80.0f,0.0f)); 
	thumbnail->setAdditiveBlending(true);
	
	bg = ImageViewRef( new ImageView(assets->getTextureByID("hb_character") ) ); 
	bg->setAlpha(0.6f);
	bg->setPosition(ci::Vec3f(100.0f,80.0f,0.0f)); 

	fullScreenBg = ImageViewRef( new ImageView(assets->getTextureByID("hb_video") ) ); 
	fullScreenBg->setScale( ci::Vec3f(1600.0f/fullScreenBg->getSize()->value().x, 1080.0f/fullScreenBg->getSize()->value().y, 0) );
	fullScreenBg->setAlpha(0.8f);

	bigImage = NULL; 

	bgBox = ImageViewRef( new ImageView(assets->getTextureByID("bgBox")) );
	
	bgBox->setScale(ci::Vec3f(1920.0f/bgBox->getSize()->value().x, 1080.0f/bgBox->getSize()->value().y,0));
	bgBox->setAlpha(0.5f);
	
	addChild(bg); 
	addChild(thumbnail); 

	active = false;
}


void DataViewDocument::disposeTempAssets()
{
	if(bigImage){
		bigImage->deleteTexture(); 
		bigImage.reset(); 
		bigImage = NULL;
	}
}

void DataViewDocument::update(float dt)
{
	InputView::update(dt);

	if(active){
		float y = bigImage->getPosition()->value().y + stick * dt * 1000;
		if( y > 0) y = 0; 
		if( y < 1080.0f - 2048.0f) y = 1080.0f - 2048.0f;


		bigImage->setPosition( ci::Vec3f( 170, y, 0) );
	}
}

DataViewDocument::~DataViewDocument(void)
{

}

#ifdef _WIN32
void DataViewDocument::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	if(gamepad.sThumbLY > 4000 || gamepad.sThumbLY < -4000){
		stick= ((float)gamepad.sThumbLY / (float)MAXSHORT);
	}else{
		stick = 0;
	}
}
#endif

void DataViewDocument::handleInput(INPUT_TYPE type)
{
	switch(type)
	{
	case BACK:
		//close video if its open. 
		if(active){
			active = false;
			Overlay::getInstance().setActive(false);
			Overlay::getInstance().getView()->removeChild(bgBox);
			Overlay::getInstance().getView()->removeChild(fullScreenBg);
			Overlay::getInstance().getView()->removeChild(bigImage);
		}
		break;

	case SELECT:
		//close video if its open. 
		if(!active){
			if(!bigImage) bigImage = ImageViewRef( new ImageView( new ci::gl::Texture(loadImage(loadAsset(assetPath))) ) );
			
			//major hacks....
			// only laskys letter dosnt need additive blending
			if(assetPath != "scenes/data/data_letter.png"){
				bigImage->setAdditiveBlending(true);
				//bigImage->setAlpha(0.8f);
			}

			active = true; 
			bigImage->setPosition( ci::Vec3f(170,0,0) );
			fullScreenBg->setPosition( ci::Vec3f(170,0,0) ); 

			Overlay::getInstance().setActive(true);
			Overlay::getInstance().getView()->addChild(bgBox);
			Overlay::getInstance().getView()->addChild(fullScreenBg);
			Overlay::getInstance().getView()->addChild(bigImage);

		}
		break;
	}
}