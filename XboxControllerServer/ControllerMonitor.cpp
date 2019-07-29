#include "ControllerMonitor.h"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;
Controller::Controller()
{
	RightTrigger = 0;
	LeftTrigger = 0;
	rightButton = 0;
	leftButton = 0;
	RightAnalog.X = 0;
	LeftAnalog.Y = 0;
	buttonA = 0;
	buttonB = 0;
	buttonX = 0;
	buttonY = 0;
	_buttons = 0;
}
void Controller::setControllerState(XINPUT_GAMEPAD gamepadState) 
{
	RightTrigger = gamepadState.bRightTrigger;
	LeftTrigger = gamepadState.bLeftTrigger;
	RightAnalog.X = gamepadState.sThumbRX;
	RightAnalog.Y = gamepadState.sThumbRY;
	LeftAnalog.X = gamepadState.sThumbLX;
	LeftAnalog.Y = gamepadState.sThumbLY;
	rightButton = leftButton = buttonA = buttonB = buttonX = buttonY = 0;
	
	switch (gamepadState.wButtons)
	{
		case XINPUT_GAMEPAD_RIGHT_SHOULDER:
			rightButton = gamepadState.wButtons;
			break;
		case XINPUT_GAMEPAD_LEFT_SHOULDER:
			leftButton = gamepadState.wButtons;
			break;
		case XINPUT_GAMEPAD_A:
			buttonA = gamepadState.wButtons;
			break;
		case XINPUT_GAMEPAD_B:
			buttonB = gamepadState.wButtons;
			break;
		case XINPUT_GAMEPAD_X:
			buttonX = gamepadState.wButtons;
			break;
		case XINPUT_GAMEPAD_Y:
			buttonY = gamepadState.wButtons;
			break;
		default:
			break;
	}
	_buttons = gamepadState.wButtons;
}


ControllerMonitor::ControllerMonitor(int user)
{
	Users = user;
	getStateResult = XInputGetState(user, &newControllerState);
	controller = new Controller();
	if (getStateResult == 0)
	{
		controller->setControllerState(newControllerState.Gamepad);
		prevControllerState = newControllerState;
		//startMonitor();
	}
	
}

void ControllerMonitor::startMonitor(void)
{
	getStateResult = XInputGetState(Users, &newControllerState);
	//emit SIGNAL(ControllerUpdate(controller));
	while (getStateResult == 0)
	{
		controller->setControllerState(newControllerState.Gamepad);
		prevControllerState = newControllerState;
		getStateResult = XInputGetState(Users, &newControllerState);
		emit ControllerUpdate(controller);
		std::this_thread::sleep_for(200ms);
	}
}

