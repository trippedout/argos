#include "InputKeyboard.h"

using namespace ci;
using namespace ci::app;

InputKeyboard::InputKeyboard(void)
{
}

InputKeyboard::~InputKeyboard(void)
{
}

void InputKeyboard::handleKeyboard(KeyEvent* ke)
{
	ci::app::console() << "handleKeyboard() " << ke->getChar() << " and: " << ke->getCode() << std::endl;

	switch(ke->getCode())
	{
	case KeyEvent::KEY_RETURN:
		mInputSignal(SELECT);
		break;

	case KeyEvent::KEY_BACKSPACE:
		mInputSignal(BACK);
		break;

	case KeyEvent::KEY_w:
		mInputSignal(LEFT_STICK_UP);
		mInputSignal(UP);
		break;

	case KeyEvent::KEY_s:
		mInputSignal(LEFT_STICK_DOWN);
		mInputSignal(DOWN);
		break;

	case KeyEvent::KEY_a:
		mInputSignal(LEFT_STICK_LEFT);
		mInputSignal(LEFT);
		break;

	case KeyEvent::KEY_d:
		mInputSignal(LEFT_STICK_RIGHT);
		mInputSignal(RIGHT);
		break;
	
	case KeyEvent::KEY_UP:
		mInputSignal(DPAD_UP);
		mInputSignal(UP);
		break;

	case KeyEvent::KEY_DOWN:
		mInputSignal(DPAD_DOWN);
		mInputSignal(DOWN);
		break;

	case KeyEvent::KEY_LEFT:
		mInputSignal(DPAD_LEFT);
		mInputSignal(LEFT);
		break;

	case KeyEvent::KEY_RIGHT:
		mInputSignal(DPAD_RIGHT);
		mInputSignal(RIGHT);
		break;

	case KeyEvent::KEY_q:
		mInputSignal(PREVIOUS);
		break;

	case KeyEvent::KEY_e:
		mInputSignal(NEXT);
		break;

	case KeyEvent::KEY_i:
		mInputSignal(PRESSED_Y);
		break;
	case KeyEvent::KEY_j:
		mInputSignal(PRESSED_X);
		break;
	case KeyEvent::KEY_k:
		mInputSignal(PRESSED_A);
		break;
	case KeyEvent::KEY_l:
		mInputSignal(PRESSED_B);
		break;

	case KeyEvent::KEY_o:
		mInputSignal(UNLOCK_ALL_DATA);
		break;

	case KeyEvent::KEY_p:
		mInputSignal(LOCK_ALL_DATA);
		break;
	}
}
