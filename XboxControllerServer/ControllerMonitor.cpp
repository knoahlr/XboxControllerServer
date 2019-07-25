#include "ControllerMonitor.h"

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
}
void::Controller::setControllerState(XINPUT_GAMEPAD gamepadState) 
{
	RightTrigger = gamepadState.bRightTrigger;
	LeftTrigger = gamepadState.bLeftTrigger;
	rightButton = 0;
	leftButton = 0;
	RightAnalog.X = gamepadState.sThumbRX;
	RightAnalog.Y = gamepadState.sThumbRY;
	LeftAnalog.X = gamepadState.sThumbLX;
	LeftAnalog.Y = gamepadState.sThumbLY;
	buttonA = 0;
	buttonB = 0;
	buttonX = 0;
	buttonY = 0;

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

void::ControllerMonitor::startMonitor(void)
{
	getStateResult = XInputGetState(Users, &newControllerState);
	emit SIGNAL(ControllerUpdate(controller));
	while (getStateResult == 0)
	{
		controller->setControllerState(newControllerState.Gamepad);
		prevControllerState = newControllerState;
		getStateResult = XInputGetState(Users, &newControllerState);
		emit SIGNAL(ControllerUpdate(controller));
	}
}

