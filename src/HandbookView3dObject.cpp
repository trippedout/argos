#include "HandbookView3dObject.h"


HandbookView3dObject::HandbookView3dObject(DataNodeRef dn, std::string assetID, Assets* assets):
	DataView3dObject(assets)
{
	dataNode = dn;
	object = boost::shared_ptr<HandbookRotationView>( new HandbookRotationView(assetID, assets->getShaderByID("rotate") ));
	
	//TODO: major hacks... scale down the spinning logos, scale up warthog and corvette. 
	if(assetID == "rotation/15_ONI" || assetID == "rotation/17_UNSC_Logo" || assetID == "rotation/16_HASTATI_Logo"){
		object->setScale(ci::Vec3f(1.0f,1.0f,1.0f)); 
	}else if(assetID == "rotation/4.Warthog" || assetID == "rotation/11.Corvette" ){
		object->setScale(ci::Vec3f(1.65f,1.65f,1.65f)); 
	}else{
		object->setScale(ci::Vec3f(1.5f,1.5f,1.5f)); 
	}
	//object->setScale(ci::Vec3f(0.8f, 0.8f, 0.8f)); 
	//object->setCenter(ci::Vec3f(0.5f, 0.5f, 0.5f));

	addChild(object); 

	stick = 0;
	speed = 1.5f;
}

void HandbookView3dObject::update(float dt)
{
	DataView3dObject::update(dt);
	//TODO: this is really ghetto... please fix this.
	float s = object->getScale()->value().x;
	
	object->setPosition( 
		ci::Vec3f( 1200.0f - object->getSize()->value().x*s/2.0f, 
			330.0f - object->getSize()->value().y*s/2.0f + 50.0f - 100.0f - 60.0f, 0) 
			+ ci::Vec3f(200.0f,185.0f,0) );
}

void HandbookView3dObject::onAnimateIn()
{
	DataView3dObject::onAnimateIn();
}

void HandbookView3dObject::onAnimateOut()
{
	DataView3dObject::onAnimateOut();
}

void HandbookView3dObject::show() 
{
	object->show(); 
}

void HandbookView3dObject::hide()
{
	object->hide(); 
}

HandbookView3dObject::~HandbookView3dObject(void)
{
}
