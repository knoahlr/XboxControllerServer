#pragma once

#include <QtWidgets/QMainWindow>
#include <QtGui>

#include <QHBoxLayout> 
#include "StatusBar.h"

#include <QtCore/qlogging.h>
#include <QMessageLogger>
//#include <Xinput.h>
#include "ControllerMonitor.h"
#include "ServerWidget.h"
#include "TcpClient.h"

class XboxControllerServer : public QMainWindow
{
	Q_OBJECT
	QThread *Listener = Q_NULLPTR;
	QThread *clientManager = Q_NULLPTR;

public:
	XboxControllerServer(QWidget *parent = Q_NULLPTR);
#pragma region GUI
	// GUI
	QWidget *centralWidget;
	QVBoxLayout *centralLayout;
	QIcon *Icon;
	DefaultStatusBar *statusBar;

	//TABS
	QTabWidget *Tabs;
	ServerWidget *serverTab = Q_NULLPTR;

#pragma endregion GUI

	//Gamepad Listener
	ControllerMonitor *gamepads;
	bool controllerDefined = false;
	int controllerRetries = 5;
	int playerID = 0;

	void initializeGUI(void);
	void initializeClient(void);
	void startListener(void);
	void launchClient(void);
	void closeEvent(QCloseEvent *event);
	int currentMode(void);
	void updateButtonFields(Controller *newGamepadState);

private:
	TcpClient *Client = Q_NULLPTR;


public slots:
	void logSlot(QString message);
	void startServer(void);
	void stopServer(void);
	void handlenewGamepadState(Controller *newGamepadState);
	void tcpResponseHandler(QString data);
	void connectionUpdate(bool connected);

signals:
	void logSignal(QString message);
	void monitor(void);
	
	//TCP Client Signals
	void tryConnect(void);
	void sendData(QByteArray data);
};



