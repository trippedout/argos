#pragma once
#include "View.h"

#include "ImageView.h"
#include "TextView.h"
#include "Scene.h"
#include "DataNode.h"


class PauseSceneInfo :	public View
{
protected:
	boost::shared_ptr<Scene> scene; 

	ImageViewRef line;
	ImageViewRef dataIcon;
	ImageViewRef oniIcon;
	ImageViewRef socialIcon;

	TextViewRef socialText;

	int numBeacons;
	float sceneTime;

public:
	PauseSceneInfo(Assets* assets, boost::shared_ptr<Scene> sc);
	~PauseSceneInfo(void);

	void checkData();
	void offsetBeacon(ci::Vec3f pos); 
	void hideBeaconText(); 

	int getNumBeacons(){return numBeacons;};
	float getSceneTime(){return sceneTime;}; 
};

