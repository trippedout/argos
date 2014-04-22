#include "DataView3dObject.h"

DataView3dObject::DataView3dObject(Assets* assets) : InputView(assets)
{

}

DataView3dObject::DataView3dObject(boost::shared_ptr<DataNode> dn, std::string assetID, Assets* assets) : InputView(assets)
{
	dataNode = dn;
	object = boost::shared_ptr<RotationView>( new RotationView(assetID, assets->getShaderByID("rotate") ));
	//object->setScale(ci::Vec3f(1.25f,1.25f,1.25f)); 

	if(assetID == "rotation/15_ONI" || assetID == "rotation/17_UNSC_Logo" || assetID == "rotation/16_HASTATI_Logo"){
		object->setScale(ci::Vec3f(1.1f,1.1f,1.1f)); 
	}else{
		object->setScale(ci::Vec3f(1.25f,1.25f,1.25f)); 
	}

	//object->setScale(ci::Vec3f(0.8f, 0.8f, 0.8f)); 
	//object->setCenter(ci::Vec3f(0.5f, 0.5f, 0.5f));

	addChild(object); 

	stick = 0;
	speed = 1.5f;
}

DataView3dObject::~DataView3dObject(void)
{

}

void DataView3dObject::disposeTempAssets()
{
	object->disposeTempAssets();
}

void DataView3dObject::onAnimateIn()
{
	//ci::app::console()<< "DataView3dObject::onAnimateIn" << std::endl; 
	delay = 0; 
	stick = 0;
	speed = 1.5f;
	object->onAnimateIn();
}

void DataView3dObject::onAnimateOut()
{
	object->onAnimateOut();
}

void DataView3dObject::handleInput(INPUT_TYPE type)
{

}

void  DataView3dObject::prerender()
{
	object->prerender();
}

void  DataView3dObject::update(float dt)
{
	float s = object->getScale()->value().x;
	//TODO: this is really ghetto... please fix this.
	object->setPosition(ci::Vec3f(400.0f + 200.0f - object->getSize()->value().x*s/2.0f, 256.0f - object->getSize()->value().y*s/2.0f + 50.0f, 0));

	InputView::update(dt);
	
	if(delay > 0)delay-=dt; 

	if(stick != 0){
		delay = 2.0f; 
		
		speed = stick * 7 * -1; 
		//if(speed > 7.0f) speed = 7.0f;
		//if(speed < -7.0f) speed = -7.0f;
		
		if( speed < 0 && speed > -1.0){
			speed = 0; 
		}else if(speed >= 0 && speed < 1.0){
			speed = 0; 
		}

	}else{
		if( delay <= 0){
			if( speed < 0 && speed > -1.5){
				speed -= dt * 10; 
			}else if(speed >= 0 && speed < 1.5){
				speed += dt * 10; 
			}
		}else{
			speed = 0; 
		}
	}

	object->setSpeed(speed);
	
}

#ifdef _WIN32
void DataView3dObject::handleGamepad(XINPUT_GAMEPAD gamepad)
{
	if(gamepad.sThumbRX > 5000 || gamepad.sThumbRX < -5000){
		stick= ((float)gamepad.sThumbRX / (float)MAXSHORT);
	}else{
		stick = 0;
	}
}
#endif