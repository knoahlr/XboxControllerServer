#include "XboxControllerServer.h"
#include "qgamepad.h"

using namespace std::chrono_literals;
XboxControllerServer::XboxControllerServer(QWidget *parent)
	: QMainWindow(parent)
{
	initializeGUI();
	initializeClient();
}
void::XboxControllerServer::initializeGUI(void)
{

	//GUI
	centralWidget = new QWidget();
	centralLayout = new QVBoxLayout();
	Icon = new QIcon("../Articles/atom.png");
	setWindowIcon(*Icon);
	statusBar = new DefaultStatusBar();

	//Top GroupBox
	gamepad = new QGroupBox("Gamepad");
	gamepadLayout = new QHBoxLayout();
	gamepad->setLayout(gamepadLayout);

	//Left Pane Box
	buttons = new QGroupBox("Buttons");
	buttonsLayout = new QFormLayout();
	buttons->setLayout(buttonsLayout);

	//Right Pane Box
	trigger_Analog = new QGroupBox("Triggers and Analog");
	trigger_AnalogLayout = new  QFormLayout();
	trigger_Analog->setLayout(trigger_AnalogLayout);

	//Middle Log groupBox
	logGroupBox = new QGroupBox("Log");
	logFrameLayout = new QVBoxLayout();
	logGroupBox->setLayout(logFrameLayout);

	//Bottom control and status groupbox
	controls_status = new QGroupBox();
	controls_statusLayout = new QHBoxLayout();
	controls_status->setLayout(controls_statusLayout);


	//Labels and line Edit
	labelA = new QLabel("A");
	labelB = new QLabel("B");
	labelX = new QLabel("X");
	labelY = new QLabel("Y");
	labelLeftTrigger = new QLabel("Left Trigger");
	labelRightAnalog = new QLabel("Right Analog");
	labelLeftAnalog = new QLabel("Left Analog");
	labelRightTrigger = new QLabel("Right Trigger");

	lineEditA = new QLineEdit();
	lineEditB = new QLineEdit();
	lineEditX = new QLineEdit();
	lineEditY = new QLineEdit();
	lineEditLeftTrigger = new QLineEdit();
	lineEditRightAnalog = new QLineEdit();
	lineEditLeftAnalog = new QLineEdit();
	lineEditRightTrigger = new QLineEdit();

	//
	logBox = new QPlainTextEdit();
	logBox->setReadOnly(true);
	testMe = new QPushButton("Start Server");

	gamepadLayout->addWidget(buttons);
	gamepadLayout->addWidget(trigger_Analog);
	gamepadLayout->addWidget(logGroupBox);
	//gamepadLayout->addWidget(controls_status);

	buttonsLayout->setWidget(0, QFormLayout::LabelRole, labelA);
	buttonsLayout->setWidget(0, QFormLayout::FieldRole, lineEditA);

	buttonsLayout->setWidget(1, QFormLayout::LabelRole, labelB);
	buttonsLayout->setWidget(1, QFormLayout::FieldRole, lineEditB);

	buttonsLayout->setWidget(2, QFormLayout::LabelRole, labelX);
	buttonsLayout->setWidget(2, QFormLayout::FieldRole, lineEditX);

	buttonsLayout->setWidget(3, QFormLayout::LabelRole, labelY);
	buttonsLayout->setWidget(3, QFormLayout::FieldRole, lineEditY);

	trigger_AnalogLayout->setWidget(0, QFormLayout::LabelRole, labelLeftTrigger);
	trigger_AnalogLayout->setWidget(0, QFormLayout::FieldRole, lineEditLeftTrigger);

	trigger_AnalogLayout->setWidget(1, QFormLayout::LabelRole, labelRightTrigger);
	trigger_AnalogLayout->setWidget(1, QFormLayout::FieldRole, lineEditRightTrigger);

	trigger_AnalogLayout->setWidget(2, QFormLayout::LabelRole, labelLeftAnalog);
	trigger_AnalogLayout->setWidget(2, QFormLayout::FieldRole, lineEditLeftAnalog);

	trigger_AnalogLayout->setWidget(3, QFormLayout::LabelRole, labelRightAnalog);
	trigger_AnalogLayout->setWidget(3, QFormLayout::FieldRole, lineEditRightAnalog);

	logFrameLayout->addWidget(logBox);
	controls_statusLayout->addWidget(testMe);

	connect(testMe, SIGNAL(clicked()), this, SLOT(startServer()));

	centralLayout->addWidget(gamepad);
	centralLayout->addWidget(controls_status);

	centralWidget->setLayout(centralLayout);
	setCentralWidget(centralWidget);
	setStatusBar(statusBar);
	resize(800, 500);

}

void::XboxControllerServer::initializeClient(void)
{
	Client = new TcpClient();
	connect(Client, SIGNAL(transactionComplete(QString)), this, SLOT(tcpResponseHandler(QString)));
	if (Client->connectToHost(mcuIP, 1000))
	{
		QString connected = QString("Connected");
		Client->writeData(connected.toUtf8());
	}
	
}
void::XboxControllerServer::startServer(void) {

	DWORD dwResult;
	XINPUT_STATE state;

	for (int retries = 0; retries <= controllerRetries; retries ++)
	{
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(playerID, &state);
		if (dwResult == ERROR_SUCCESS)
		{
			startListener();
			return;
		}
		else 
		{
			logSlot(QString("Controller not found. Waiting 2s"));
			std::this_thread::sleep_for(2s);
			QApplication::processEvents();
		}
	}
	logSlot(QString("No Controller connected to system"));
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
		emit monitor();
		initializeClient();
	}
	else 
	{
		logSlot("Controller already found");
	}

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

void::XboxControllerServer::tcpResponseHandler(QString data)
{
	logSlot(data);
}
void::XboxControllerServer::closeEvent(QCloseEvent *event)
{
	Listener.quit();
}


