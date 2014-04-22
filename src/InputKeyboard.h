#pragma once

#include "InputBase.h"

class InputKeyboard : public InputBase
{
public:
	InputKeyboard(void);
	~InputKeyboard(void);
	
	void						handleKeyboard(ci::app::KeyEvent* ke);
};

