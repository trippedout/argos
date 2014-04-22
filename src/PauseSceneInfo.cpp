#include "PauseSceneInfo.h"

PauseSceneInfo::PauseSceneInfo(Assets* assets, boost::shared_ptr<Scene> sc):View(assets)
{
	scene = sc;

	line = ImageViewRef( new ImageView(assets->getTextureByID("pause_timeline_bar")) ); 
	line->setPosition( ci::Vec3f(0, 10.0f,0) );

	dataIcon = ImageViewRef( new ImageView(assets->getTextureByID("pause_icon_data")) ); 
	dataIcon->setPosition( ci::Vec3f(7.0f, 30.0f,0) );

	oniIcon = ImageViewRef( new ImageView(assets->getTextureByID("pause_icon_oni")) ); 
	oniIcon->setPosition( ci::Vec3f(7.0f,0,0) );

	std::vector<boost::shared_ptr<BeaconData>> beacons = *scene->getBeacons(); 
	sceneTime = scene->getStart();

	int size = beacons.size(); 
	numBeacons = size; 

	if( size > 0 ){
		socialIcon = ImageViewRef( new ImageView(assets->getTextureByID(beacons[0]->assetID)) ); 
		socialIcon->setPosition( ci::Vec3f(7.0f,260.0f,0) );
		addChild(socialIcon);

		socialText = TextViewRef(new TextView(beacons[0]->message, assets->getFontByID("pause_beacon"), ci::ColorA(0.66f, 0.59f, 0.3f, 1.0f)));
		socialText->setPosition( ci::Vec3f(7.0f + 32.0f,260.0f + 7.0f,0) );
		addChild(socialText);
	}

	checkData();
}

void PauseSceneInfo::checkData()
{
	if(contains(line)) removeChild(line);
	if(contains(dataIcon)) removeChild(dataIcon);
	//if(contains(oniIcon)) removeChild(oniIcon);

	bool oni = scene->isOni(); 
	bool data = !scene->isLocked(); 

	if( oni || data ){
		if(data) addChild(line);
		
		if(oni) addChild(oniIcon);
		if(data) addChild(dataIcon);
	}

}

void PauseSceneInfo::offsetBeacon(ci::Vec3f pos)
{
	if(socialText) socialText->setPosition( ci::Vec3f(7.0f + 32.0f,260.0f + 7.0f,0) + pos );
}

void PauseSceneInfo::hideBeaconText(){
	if(socialText) removeChild(socialText);
}

PauseSceneInfo::~PauseSceneInfo(void)
{
}
