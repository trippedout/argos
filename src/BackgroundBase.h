#pragma once

#include "View.h"

class BackgroundBase : public View 
{
public:
	BackgroundBase(Assets* assets);
	virtual ~BackgroundBase(void);

	virtual void handleInputFromView();

	virtual void update(float dt);
};
