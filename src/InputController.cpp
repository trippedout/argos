#include "InputController.h"

using namespace ci;
using namespace ci::app;

InputController::InputController(void)
{
	m_pVoiceCommands = new InputVoiceCommand;
	m_pKeyboard = new InputKeyboard;
#ifdef _WIN32
	m_pXboxController = new InputXBoxController;
#endif
}

InputController::~InputController(void)
{
	delete m_pVoiceCommands;
	m_pVoiceCommands = NULL;

	delete m_pKeyboard;
	m_pKeyboard = NULL;
    
#ifdef _WIN32
	delete m_pXboxController;
	m_pXboxController = NULL;
#endif
}

void InputController::setup()
{
	m_pVoiceCommands->setup();
	m_pVoiceCommands->registerSignal(boost::bind(&InputController::handleInput, this, boost::arg<1>::arg()));

	m_pKeyboard->setup();
	m_pKeyboard->registerSignal(boost::bind(&InputController::handleInput, this, boost::arg<1>::arg()));
    
#ifdef _WIN32
	m_pXboxController->setup();
	m_pXboxController->registerSignal(boost::bind(&InputController::handleInput, this, boost::arg<1>::arg()));
#endif
}

void InputController::update(float dt)
{
	m_pVoiceCommands->update(dt);
	m_pKeyboard->update();
#ifdef _WIN32
	m_pXboxController->update();
#endif
}

void InputController::render(ci::TextLayout* infoText)
{
#ifdef _WIN32
	if(Settings::DEBUG)
	{
		infoText->addLine( "controller connected: " + toString(m_pXboxController->getController()->IsConnected()) );
	
		if(m_pXboxController->getController()->IsConnected()){
			infoText->addLine( " leftStick: " + toString(getGamepad().sThumbLX)+ " , " + toString(getGamepad().sThumbLY) );
			infoText->addLine( " triggers: " + toString((int)getGamepad().bLeftTrigger)+ " , " + toString((int)getGamepad().bRightTrigger) );
			infoText->addLine( " left: " + toString( (int)(getGamepad().wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ) );
			infoText->addLine( " right: " + toString( (int)(getGamepad().wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ) );
			//infoText->addLine( " x: " + toString(getGamepad().wButtons & XINPUT_GAMEPAD_X) );
			//infoText->addLine( " y: " + toString(getGamepad().wButtons & XINPUT_GAMEPAD_Y) );
		}
	}
#endif
}

void InputController::handleInput(INPUT_TYPE type)
{
	mInputSignal(type);
}

InputKeyboard* InputController::getKeyboardInput()
{
	return m_pKeyboard;
}

bool InputController::gamepadConnected()
{
#ifdef _WIN32
	return  m_pXboxController->getController()->IsConnected();
#else
    return false;
#endif
}

#ifdef _WIN32
XINPUT_GAMEPAD InputController::getGamepad()
{
	return m_pXboxController->getController()->GetState().Gamepad;
}
#endif