#pragma once

#include "cinder/app/App.h"
#include "SignalDispatcher.h"
#include "InputTypes.h"

//using namespace ci;
//using namespace ci::app;
//using namespace std;

class InputBase : public SignalDispatcher
{
public:
	InputBase(void);
	~InputBase(void);

	void	setup();
	void	update();

protected:

};

