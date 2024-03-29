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
	Icon = new QIcon(":/XboxControllerServer/Resources/atom.png");
	centralLayout = new QVBoxLayout();
	statusBar = new DefaultStatusBar();

	Tabs = new QTabWidget();
	serverTab = new ServerWidget();
	
	createActions();
	createTrayIcon();

	Tabs->addTab(serverTab, tr("Server Widget"));
	connect(serverTab->StartServer, SIGNAL(clicked()), this, SLOT(startServer()));
	connect(serverTab->StopServer, SIGNAL(clicked()), this, SLOT(stopServer()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	centralLayout->addWidget(Tabs);
	centralWidget->setLayout(centralLayout);
	setCentralWidget(centralWidget);
	setStatusBar(statusBar);
	resize(500, 500);

}

void XboxControllerServer::initializeClient(void)
{
	if (Client == Q_NULLPTR)
	{
		QString ip = serverTab->ipLineEdit->text();
		int port = serverTab->portLineEdit->text().toInt();

		clientManager = new QThread(this);
		Client = new TcpClient(ip, port);
		Client->moveToThread(clientManager);
		connect(Client, SIGNAL(transactionComplete(QString)), this, SLOT(tcpResponseHandler(QString)));
		connect(Client, SIGNAL(deviceStateUpdate(bool)), this, SLOT(connectionUpdate(bool)));
		connect(this, SIGNAL(tryConnect(void)), Client, SLOT(connectToHost(void)));
		connect(this, SIGNAL(sendData(QByteArray)), Client, SLOT(writeData(QByteArray)));
		clientManager->start();
		emit tryConnect();
	
	}
}

void XboxControllerServer::createTrayIcon()
{
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(maximizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(*Icon);
	trayIcon->setVisible(true);
	trayIcon->setContextMenu(trayIconMenu);
}

void XboxControllerServer::createActions()
{
	minimizeAction = new QAction(tr("Mi&nimize"), this);
	connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

	maximizeAction = new QAction(tr("Ma&ximize"), this);
	connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

	restoreAction = new QAction(tr("&Restore"), this);
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

	quitAction = new QAction(tr("&Quit"), this);
	bool dummy = connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void XboxControllerServer::startServer(void) {

	DWORD dwResult;
	XINPUT_STATE state;
	ServerWidget::options selectedCase = (ServerWidget::options)serverTab->currentMode();

	switch (selectedCase)
	{
		case ServerWidget::MCU:
		case ServerWidget::Server:
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
		case ServerWidget::Debug:
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

	serverTab->logBox->appendPlainText(message);

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
	ServerWidget::options selectedCase = (ServerWidget::options)serverTab->currentMode();
	switch (selectedCase)
	{
	case ServerWidget::MCU:
		initializeClient();
		break;
	case ServerWidget::Server:
		break;
	}
}

void XboxControllerServer::handlenewGamepadState(Controller *newGamepadState)
{
	ServerWidget::options selectedCase = (ServerWidget::options)serverTab->currentMode();
	QString message = QString("Right Analog: %1, %2\nLeft Analog: %3, %4\nRight Trigger: %5 Left Trigger: %6\nButtons: %7")\
		.arg(QString::number(newGamepadState->RightAnalog.X), QString::number(newGamepadState->RightAnalog.Y), \
			QString::number(newGamepadState->LeftAnalog.X), QString::number(newGamepadState->LeftAnalog.Y),
			QString::number(newGamepadState->RightTrigger), QString::number(newGamepadState->LeftTrigger),
			QString::number(newGamepadState->_buttons));
	updateButtonFields(newGamepadState);
	switch (selectedCase)
	{
		case ServerWidget::Server:
			logSlot(message);
			break;
		case ServerWidget::MCU:
			(Client == Q_NULLPTR) ? false : emit sendData(message.toUtf8());
			break;
		default:
			logSlot("Switch Case Failed");
			break;
	}
}

void XboxControllerServer::tcpResponseHandler(QString data)
{
	logSlot(data);
}

void XboxControllerServer::updateButtonFields(Controller *newGamepadState)
{
	serverTab->lineEditA->setText(QString::number(newGamepadState->buttonA));
	serverTab->lineEditB->setText(QString::number(newGamepadState->buttonB));
	serverTab->lineEditX->setText(QString::number(newGamepadState->buttonX));
	serverTab->lineEditY->setText(QString::number(newGamepadState->buttonY));
	serverTab->lineEditLeftTrigger->setText(QString::number(newGamepadState->LeftTrigger));
	serverTab->lineEditRightAnalog->setText(QString("%1, %2").arg(QString::number(newGamepadState->RightAnalog.X), QString::number(newGamepadState->RightAnalog.Y)));
	serverTab->lineEditLeftAnalog->setText(QString("%1, %2").arg(QString::number(newGamepadState->LeftAnalog.X), QString::number(newGamepadState->LeftAnalog.Y)));
	serverTab->lineEditRightTrigger->setText(QString::number(newGamepadState->RightTrigger));
}

void XboxControllerServer::connectionUpdate(bool connected)
{
	if (connected)
	{
		serverTab->ServerConnected->setText("Connected");
		serverTab->ServerConnected->setStyleSheet("QLabel { background-color : Green; }");
	}
	else if(!connected)
	{
		serverTab->ServerConnected->setText("Disconnected");
		serverTab->ServerConnected->setStyleSheet("QLabel { background-color : Red; }");
	}

}

void XboxControllerServer::closeEvent(QCloseEvent *event)
{
	if (trayIcon->isVisible()) 
	{
		QMessageBox::information(this, tr("QUIT"),
			tr("The program will keep running in the "
				"system tray. To terminate the program, "
				"choose <b>Quit</b> in the context menu "
				"of the system tray entry."));
		hide();
		event->ignore();
	}
	else //currently not called. Need a way to tie action to QCloseEvent signal
	{
		stopServer();
	}
}




