#include "InputXBoxController.h"

#ifdef _WIN32
InputXBoxController::InputXBoxController(void)
{
	m_Controller = new CXBOXController(1);
}

InputXBoxController::~InputXBoxController(void)
{
	delete m_Controller;
	m_Controller = NULL;
}

CXBOXController* InputXBoxController::getController()
{
	return m_Controller;
}

void InputXBoxController::setup()
{
	prevLS = ci::Vec2i(0,0);
	lastA = false;
	lastB = false;
	lastX = false;
	lastY = false;
	lastUp = false;
	lastDown = false;
	lastLeft = false;
	lastRight = false;
	lastLB = false;
	lastRB = false;
	lastStart = false;
	lastBack = false;
}

void InputXBoxController::update()
{
	const int t = 24000;

	if(m_Controller->IsConnected())
	{
		if( m_Controller->GetState().Gamepad.sThumbLY > t && prevLS.y < t )
		{
			mInputSignal(LEFT_STICK_UP);
			mInputSignal(UP);
		}
		if( m_Controller->GetState().Gamepad.sThumbLY < -t && prevLS.y > -t  )
		{
			mInputSignal(LEFT_STICK_DOWN);
			mInputSignal(DOWN);
		}
		if( m_Controller->GetState().Gamepad.sThumbLX < -t && prevLS.x > -t )
		{
			mInputSignal(LEFT_STICK_LEFT);
			mInputSignal(LEFT);
		}
		if( m_Controller->GetState().Gamepad.sThumbLX > t && prevLS.x < t )
		{
			mInputSignal(LEFT_STICK_RIGHT);
			mInputSignal(RIGHT);
		}

		if( m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A && !lastA ) 
		{
			mInputSignal(PRESSED_A); 
			mInputSignal(SELECT);
		}
		if( m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B && !lastB ) 
		{
			mInputSignal(PRESSED_B); 
			mInputSignal(BACK);
		}
		if( m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X && !lastX ) 
		{
			mInputSignal(PRESSED_X); 
		}
		if( m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y && !lastY ) 
		{
			mInputSignal(PRESSED_Y); 
		}

		//DPAD ONLY TRIGGERS DPAD 
		if( m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP && !lastUp ) 
		{
			mInputSignal(DPAD_UP);
		}
		if( m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN && !lastDown ) 
		{
			mInputSignal(DPAD_DOWN);
		}
		if( m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT && !lastLeft ) 
		{
			mInputSignal(DPAD_LEFT); 
		}
		if( m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT && !lastRight ) 
		{
			mInputSignal(DPAD_RIGHT);
		}

		if( m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && !lastLB ) 
		{
			mInputSignal(LEFT_SHOULDER);
			mInputSignal(PREVIOUS);			
		}

		if( m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && !lastRB ) 
		{
			mInputSignal(RIGHT_SHOULDER);
			mInputSignal(NEXT);	
		}

		if( m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK && !lastBack ) 
		{
			mInputSignal(BACK);
		}

		if( m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START && !lastStart ) 
		{
			mInputSignal(START);
			mInputSignal(SELECT);
		}

		prevLS.x = m_Controller->GetState().Gamepad.sThumbLX;
		prevLS.y = m_Controller->GetState().Gamepad.sThumbLY;

		lastA =  m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A;
		lastB =  m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B;
		lastX =  m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X;
		lastY =  m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y;

		lastUp =  m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
		lastDown =  m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
		lastLeft =  m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
		lastRight =  m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

		lastLB = m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
		lastRB = m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
		
		lastBack = m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
		lastStart = m_Controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START;
	}
}
#endif 
