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
		float X;
		float Y;
	};
	Analog RightAnalog;
	Analog LeftAnalog;
	float RightTrigger;
	float LeftTrigger;
	float buttonA;
	float buttonB;
	float buttonX;
	float buttonY;
	float leftButton;
	float rightButton;
	void setControllerState(XINPUT_GAMEPAD gamepadState);
};

class ControllerMonitor :public QObject
{
	
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
	void ControllerUpdate(Controller *newStates);
};

