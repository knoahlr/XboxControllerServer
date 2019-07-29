#pragma once
#pragma comment(lib, "XInput.lib")
#include <Windows.h>
#include <Xinput.h>
#include <QObject>

class Controller: public QObject
{
public:
	Controller();
	struct Analog {
		BYTE X;
		BYTE Y;
	};
	Analog RightAnalog;
	Analog LeftAnalog;
	SHORT RightTrigger;
	SHORT LeftTrigger;
	float buttonA;
	float buttonB;
	float buttonX;
	float buttonY;
	float leftButton;
	float rightButton;
	WORD _buttons;
	void setControllerState(XINPUT_GAMEPAD gamepadState);
};

class ControllerMonitor :public QObject
{
	Q_OBJECT

public:
	ControllerMonitor(int user);
	XINPUT_STATE newControllerState;
	XINPUT_STATE prevControllerState;
	Controller *controller;
	DWORD getStateResult;
	int Users;

public slots:
	void  startMonitor(void);
signals:
	void ControllerUpdate(Controller *newGamepadStates);
};

