#pragma once


#ifdef _WIN32

#include "InputBase.h"
#include "CXBOXController.h"

class InputXBoxController :	public InputBase
{
public:
	InputXBoxController(void);
	~InputXBoxController(void);
	
	void	setup();
	void	update();

	CXBOXController*		getController();

    
private:
	CXBOXController*		m_Controller;

	ci::Vec2i prevLS;
	bool lastA, lastB, lastX, lastY;
	bool lastUp, lastDown, lastLeft, lastRight;
	bool lastRB, lastLB; 
	bool lastStart, lastBack; 
};

#endif

