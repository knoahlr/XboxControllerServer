#pragma once
#pragma comment(lib, "XInput.lib")
#include <Xinput.h>
class ControllerMonitor
{

public:
	XINPUT_STATE newControllerState;
	XINPUT_STATE prevControllerState;

	XINPUT_GAMEPAD controllerStateUpdate(void);

};

class Controller
{
public:

};