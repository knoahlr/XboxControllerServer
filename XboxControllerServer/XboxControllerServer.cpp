#include "XboxControllerServer.h"
#include "qgamepad.h"
XboxControllerServer::XboxControllerServer(QWidget *parent)
	: QMainWindow(parent)
{
	centralWidget = new QWidget();
	centralLayout = new QVBoxLayout();
	Icon = new QIcon("../Articles/atom.png");
	setWindowIcon(*Icon);

	logGroupBox = new QGroupBox("Log");
	logFrameLayout = new QVBoxLayout();
	logGroupBox->setLayout(logFrameLayout);

	logBox = new QPlainTextEdit();
	logBox->setReadOnly(true);
	testMe = new QPushButton("Test Me");
	connect(testMe, SIGNAL(clicked()), this, SLOT(getControllerState()));

	logFrameLayout->addWidget(logBox);
	logFrameLayout->addWidget(testMe);

	centralLayout->addWidget(logGroupBox);
	centralWidget->setLayout(centralLayout);
	setCentralWidget(centralWidget);
	resize(800, 500);

}
void::XboxControllerServer::getControllerState(void) {

	DWORD dwResult;
	XINPUT_STATE state;
	int INPUT_DEADZONE = 65335;
	int controllerCount = 1;

	for (DWORD i = 0; i < XUSER_MAX_COUNT && i < controllerCount; i++)
	{
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &state);
		if (dwResult == ERROR_SUCCESS)
		{
			float LX = state.Gamepad.sThumbLX;
			float LY = state.Gamepad.sThumbLY;

			//determine how far the controller is pushed
			float magnitude = sqrt(LX*LX + LY * LY);

			//determine the direction the controller is pushed
			float normalizedLX = LX / magnitude;
			float normalizedLY = LY / magnitude;

			float normalizedMagnitude = 0;

			//check if the controller is outside a circular dead zone
			if (magnitude > INPUT_DEADZONE)
			{
				//clip the magnitude at its expected maximum value
				if (magnitude > 32767) magnitude = 32767;

				//adjust magnitude relative to the end of the dead zone
				magnitude -= INPUT_DEADZONE;

				//optionally normalize the magnitude with respect to its expected range
				//giving a magnitude value of 0.0 to 1.0
				normalizedMagnitude = magnitude / (32767 - INPUT_DEADZONE);
				logSlot(QString::fromStdString(std::string("%d", normalizedMagnitude)));
			}
			else //if the controller is in the deadzone zero out the magnitude
			{
				logSlot(QString("%1, %2").arg(QString::number(LX), QString::number(LY)));
				magnitude = 0.0;
				normalizedMagnitude = 0.0;
			}
		}
	}

}

void::XboxControllerServer::logSlot(QString message)
{

	logBox->appendPlainText(message);

}


