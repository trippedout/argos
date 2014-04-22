#pragma once

#include "DisplayObjectContainer.h"
#include "BugElement.h"
#include "Assets.h"
#include "ImageView.h"
#include "TextView.h"

typedef boost::shared_ptr< BugElement > BugElemDisplayObjectRef;

class Bug :
	public DisplayObjectContainer
{
public:
	Bug(Assets* assets);
	~Bug(void);

	float angle; 
	ci::Vec3f blue, yellow; 

	virtual void update(float dt); 

	void hide(); 
	void idle(); 
	void progress(); 

	void pulseUp(bool isOni); 
	void pulseDown(bool isOni); 
	void pulseLeft(bool isOni); 
	void pulseRight(bool isOni); 

	void voiceOn(); 
	void voiceSuccess(); 
	void voiceOff(); 

	void selectUp(); 
	void selectDown(); 
	void selectLeft(); 
	void selectRight(); 

	bool isVoiceActive(){return voiceActive;}; 

protected: 
	enum BugState{IDLE, HIDDEN, PROGRESS};
	BugState state; 

	void checkProgress(); 
	void pulseOni();
	void pulseDirection(bool isOni, int arrow); 

	DataController* dataController; 

	ci::Vec3f itemColor, characterColor, locationColor; 
	int itemsUnlocked, charactersUnlocked, locationsUnlocked, onisUnlocked; 

	std::vector<BugElemDisplayObjectRef> bars;
	std::vector<BugElemDisplayObjectRef> arrows; 
	std::vector<ImageViewRef> dots; 

	ImageViewRef base; 
	ImageViewRef progressBase; 
	ImageViewRef progressBG;
	BugElemDisplayObjectRef oniGlow; 

	TextViewRef item;
	TextViewRef character;
	TextViewRef location;
	TextViewRef oni;

	TextViewRef itemNum;
	TextViewRef characterNum;
	TextViewRef locationNum;
	TextViewRef oniNum;

	std::vector<ImageViewRef> onis; 
	std::vector<ImageViewRef> glowDots; 

	bool voiceActive; 

};
