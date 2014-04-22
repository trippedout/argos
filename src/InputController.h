#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Utilities.h"
#include "cinder/Text.h"

#include "SignalDispatcher.h"
#include "InputVoiceCommand.h"
#include "InputXBoxController.h"
#include "InputKeyboard.h"

#include "Settings.h"


class InputController : public SignalDispatcher
{
public:
	InputController(void);
	~InputController(void);

	void						setup();
	void						update(float dt);
	void						render(ci::TextLayout* infoText);
	
	bool						gamepadConnected();
	InputKeyboard*				getKeyboardInput();

#ifdef _WIN32
    XINPUT_GAMEPAD				getGamepad();
#endif

private:

	InputVoiceCommand*			m_pVoiceCommands;
	InputKeyboard*				m_pKeyboard;
    
#ifdef _WIN32
	InputXBoxController*		m_pXboxController;
#endif
    
	void						handleInput(INPUT_TYPE type);
};

