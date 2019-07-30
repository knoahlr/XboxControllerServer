#include "XboxControllerServer.h"
#include "qgamepad.h"

using namespace std::chrono_literals;
XboxControllerServer::XboxControllerServer(QWidget *parent)
	: QMainWindow(parent)
{
	initializeGUI();
}
void XboxControllerServer::initializeGUI(void)
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
	controls_statusLayout = new QGridLayout();
	controls_status->setLayout(controls_statusLayout);

	serverMode = new QGroupBox("Mode");
	serverModeLayout = new QVBoxLayout();
	serverMode->setLayout(serverModeLayout);

	controlServer = new QGroupBox("Server Control");
	controlServerLayout = new QHBoxLayout();
	controlServer->setLayout(controlServerLayout);


	//Labels and line Edit
	labelA = new QLabel("A");
	labelB = new QLabel("B");
	labelX = new QLabel("X");
	labelY = new QLabel("Y");
	labelLeftTrigger = new QLabel("Left Trigger");
	labelRightAnalog = new QLabel("Right Analog");
	labelLeftAnalog = new QLabel("Left Analog");
	labelRightTrigger = new QLabel("Right Trigger");
	ipLabel = new QLabel("IP");
	portLabel = new QLabel("Port");

	lineEditA = new QLineEdit();
	lineEditB = new QLineEdit();
	lineEditX = new QLineEdit();
	lineEditY = new QLineEdit();
	lineEditLeftTrigger = new QLineEdit();
	lineEditRightAnalog = new QLineEdit();
	lineEditLeftAnalog = new QLineEdit();
	lineEditRightTrigger = new QLineEdit();
	ipLineEdit = new QLineEdit();
	portLineEdit = new QLineEdit();

	//
	logBox = new QPlainTextEdit();
	logBox->setReadOnly(true);
	StartServer = new QPushButton("Start Server");
	StopServer = new QPushButton("Stop Server");

	//serverMode
	serverModeOptions = new QComboBox();
	QStringList items;
	items << QMetaEnum::fromType<options>().valueToKey(Server) 
		<< QMetaEnum::fromType<options>().valueToKey(MCU) 
		<< QMetaEnum::fromType<options>().valueToKey(Debug);

	serverModeOptions->addItems(items);
	ServerConnected = new QLabel("Disconnected");
	ServerConnected->setAlignment(Qt::AlignCenter);
	ServerConnected->setStyleSheet("QLabel { background-color : red; }");

	gamepadLayout->addWidget(buttons);
	gamepadLayout->addWidget(trigger_Analog);
	gamepadLayout->addWidget(logGroupBox);

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

	controlServerLayout->addWidget(StartServer);
	controlServerLayout->addWidget(StopServer);
	serverModeLayout->addWidget(serverModeOptions);
	serverModeLayout->addWidget(ServerConnected);

	logFrameLayout->addWidget(logBox);
	controls_statusLayout->addWidget(ipLabel, 0, 0);
	controls_statusLayout->addWidget(ipLineEdit, 0, 1);
	controls_statusLayout->addWidget(portLabel, 0, 2);
	controls_statusLayout->addWidget(portLineEdit, 0, 3);
	controls_statusLayout->addWidget(controlServer, 1, 0,1,2);
	controls_statusLayout->addWidget(serverMode, 1,2,1,2);


	connect(StartServer, SIGNAL(clicked()), this, SLOT(startServer()));
	connect(StopServer, SIGNAL(clicked()), this, SLOT(stopServer()));

	centralLayout->addWidget(gamepad);
	centralLayout->addWidget(controls_status);

	centralWidget->setLayout(centralLayout);
	setCentralWidget(centralWidget);
	setStatusBar(statusBar);
	resize(500, 500);

}

void XboxControllerServer::initializeClient(void)
{
	if (Client == Q_NULLPTR)
	{
		clientManager = new QThread(this);

		Client = new TcpClient();
		Client->moveToThread(clientManager);
		connect(Client, SIGNAL(transactionComplete(QString)), this, SLOT(tcpResponseHandler(QString)));
		connect(Client, SIGNAL(deviceStateUpdate(bool)), this, SLOT(connectionUpdate(bool)));
		connect(this, SIGNAL(tryConnect(QString, int)), Client, SLOT(connectToHost(QString, int)));
		connect(this, SIGNAL(sendData(QByteArray)), Client, SLOT(writeData(QByteArray)));
		clientManager->start();
		emit tryConnect(mcuIP, 1000);
	
	}
}
void XboxControllerServer::startServer(void) {

	DWORD dwResult;
	XINPUT_STATE state;
	options selectedCase = (options)currentMode();

	switch (selectedCase)
	{
		case MCU:
		case Server:
			for (int retries = 0; retries <= controllerRetries; retries++)
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
		case Debug:
			startListener();
			return;
	}
}

void XboxControllerServer::stopServer(void)
{
	if (Client != Q_NULLPTR)
	{
		Client->disconnect();
		Client->deleteLater();
		Client = Q_NULLPTR;
		QApplication::processEvents();
		connectionUpdate(false);

		//TODO: Add Status Bar Updater
		if (clientManager != Q_NULLPTR)
		{
			clientManager->exit();
			try
			{
				if (!clientManager->wait(3000))
				{
					clientManager->terminate();
					clientManager->wait(3000);
					//std::this_thread::sleep_for(3000ms);
					controllerDefined = false;
					clientManager = Q_NULLPTR;
				}
			}
			catch (const std::exception& e)
			{
				logSlot(QString(e.what()));
			}
		}
	}
	//TODO: Add Status Bar Updater
	if (Listener != Q_NULLPTR)
	{
		Listener->exit();
		try
		{
			if(!Listener->wait(3000))
			{
				Listener->terminate();
				Listener->wait(3000);
				controllerDefined = false;				
				Listener = Q_NULLPTR;
			}
		}
		catch (const std::exception& e)
		{
			logSlot(QString(e.what()));
		}
	}
}

void XboxControllerServer::logSlot(QString message)
{

	logBox->appendPlainText(message);

}
void XboxControllerServer::startListener(void) 
{
	if (!controllerDefined && Listener == Q_NULLPTR)
	{
		Listener = new QThread(this);
		gamepads = new ControllerMonitor(0);
		gamepads->moveToThread(Listener);
		connect(this, SIGNAL(monitor()), gamepads, SLOT(startMonitor()));
		connect(gamepads, SIGNAL(ControllerUpdate(Controller*)), this, SLOT(handlenewGamepadState(Controller *)));
		Listener->start();
		controllerDefined = true;
		emit monitor();
		
		launchClient();
	}
	else 
	{
		logSlot("Controller already found");
		Client == Q_NULLPTR ? launchClient() : false;
	}

}

void XboxControllerServer::launchClient(void)
{
	options selectedCase = (options)currentMode();
	switch (selectedCase)
	{
	case MCU:
		initializeClient();
		break;
	case Server:
		break;
	}
}

void XboxControllerServer::handlenewGamepadState(Controller *newGamepadState)
{
	options selectedCase = (options)currentMode();
	QString message = QString("Right Analog: %1, %2\nLeft Analog: %3, %4\nRight Trigger: %5 Left Trigger: %6\nButtons: %7")\
		.arg(QString::number(newGamepadState->RightAnalog.X), QString::number(newGamepadState->RightAnalog.Y), \
			QString::number(newGamepadState->LeftAnalog.X), QString::number(newGamepadState->LeftAnalog.Y),
			QString::number(newGamepadState->RightTrigger), QString::number(newGamepadState->LeftTrigger),
			QString::number(newGamepadState->_buttons));
	updateButtonFields(newGamepadState);
	switch (selectedCase)
	{
		case Server:
			logSlot(message);
			break;
		case MCU:
			(Client == Q_NULLPTR) ? false : emit sendData(message.toUtf8());
			break;
		default:
			logSlot("Switch Case Failed");
			break;
	}
}
int XboxControllerServer::currentMode(void)
{
	QString selectedText = serverModeOptions->itemText(serverModeOptions->currentIndex());
	auto metaEnum = QMetaEnum::fromType<options>();
	return (options)metaEnum.keyToValue(selectedText.toLocal8Bit().data());
}

void XboxControllerServer::tcpResponseHandler(QString data)
{
	logSlot(data);
}
void XboxControllerServer::closeEvent(QCloseEvent *event)
{
	stopServer();
	QMainWindow::closeEvent(event);
}

void XboxControllerServer::updateButtonFields(Controller *newGamepadState)
{
	lineEditA->setText(QString::number(newGamepadState->buttonA));
	lineEditB->setText(QString::number(newGamepadState->buttonB));
	lineEditX->setText(QString::number(newGamepadState->buttonX));
	lineEditY->setText(QString::number(newGamepadState->buttonY));
	lineEditLeftTrigger->setText(QString::number(newGamepadState->LeftTrigger));
	lineEditRightAnalog->setText(QString("%1, %2").arg(QString::number(newGamepadState->RightAnalog.X), QString::number(newGamepadState->RightAnalog.Y)));
	lineEditLeftAnalog->setText(QString("%1, %2").arg(QString::number(newGamepadState->LeftAnalog.X), QString::number(newGamepadState->LeftAnalog.Y)));
	lineEditRightTrigger->setText(QString::number(newGamepadState->RightTrigger));
}

void XboxControllerServer::connectionUpdate(bool connected)
{
	if (connected)
	{
		ServerConnected->setText("Connected");
		ServerConnected->setStyleSheet("QLabel { background-color : Green; }");
	}
	else if(!connected)
	{
		ServerConnected->setText("Disconnected");
		ServerConnected->setStyleSheet("QLabel { background-color : Red; }");
	}

}