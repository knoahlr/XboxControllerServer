#include "XboxControllerServer.h"
#include "qgamepad.h"

XboxControllerServer::XboxControllerServer(QWidget *parent)
	: QMainWindow(parent)
{
	controllerDefined = false;
	centralWidget = new QWidget();
	centralLayout = new QVBoxLayout();
	Icon = new QIcon("../Articles/atom.png");
	setWindowIcon(*Icon);

	logGroupBox = new QGroupBox("Log");
	logFrameLayout = new QVBoxLayout();
	logGroupBox->setLayout(logFrameLayout);

	logBox = new QPlainTextEdit();
	logBox->setReadOnly(true);
	testMe = new QPushButton("Start Server");
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

	startListener();
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
void::XboxControllerServer::startListener(void) 
{
	if (!controllerDefined)
	{
		gamepads = new ControllerMonitor(0);
		gamepads->moveToThread(&Listener);
		connect(this, SIGNAL(monitor()), gamepads, SLOT(startMonitor()));
		connect(gamepads, SIGNAL(ControllerUpdate(Controller*)), this, SLOT(handleNewState(Controller *)));
		Listener.start();
		controllerDefined = true;
	}
	emit monitor();
}

void::XboxControllerServer::handleNewState(Controller *newState)
{
	logSlot(QString("Right Analog: %1, %2\nLeft Analog: %3, %4\nRight Trigger: %5 Left Trigger: %6\nButtons: %7")\
	.arg(QString::number(newState->RightAnalog.X), QString::number(newState->RightAnalog.Y),\
		QString::number(newState->LeftAnalog.X), QString::number(newState->LeftAnalog.Y),
		QString::number(newState->RightTrigger), QString::number(newState->LeftTrigger),
		QString::number(newState->_buttons)
		));
}

void::XboxControllerServer::closeEvent(QCloseEvent *event)
{
	Listener.quit();
}


