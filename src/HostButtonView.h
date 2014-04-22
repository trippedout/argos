#pragma once

#include "ARGOSButtonView.h"
#include "SpriteSheetView.h"

typedef boost::shared_ptr<class HostButtonView> HostButtonViewRef;

class HostButtonView :	public ARGOSButtonView
{
protected:
	enum HostButtonState {IDLE, PENDING, ACCEPTED, DECLINED};
	ci::Vec3f red, green, blue, yellow;

	bool invited, leader; 

	float acceptTimer, rot; 
	ImageViewRef iconBG1; 
	ImageViewRef iconBG2; 
	ImageViewRef icon; 
	SpriteSheetViewRef spinner;

	TextViewRef leaderText; 

	HostButtonState state; 
	
public:
	HostButtonView(Assets* assets, std::string copy, std::string iconID, bool lead);
	~HostButtonView(void);

	virtual void selectView(bool select);
	virtual void activate(); 
	virtual void deactivate(); 

	virtual void update(float val); 
	
	bool getIsInvited() { return invited; };
};

