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
			logSlot(QString("Left Analog:%1 ,%2 Right Analog:%3, %4\n Right and Left Trigger:%5, %6")\
				.arg (QString::number(state.Gamepad.sThumbLX), QString::number(state.Gamepad.sThumbLY), \
					QString::number(state.Gamepad.sThumbRX), QString::number(state.Gamepad.sThumbRY),\
					QString::number(state.Gamepad.bRightTrigger), QString::number(state.Gamepad.bLeftTrigger)));		
		}
	}

}

void::XboxControllerServer::logSlot(QString message)
{

	logBox->appendPlainText(message);

}


