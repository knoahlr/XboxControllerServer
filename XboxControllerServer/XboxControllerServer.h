#pragma once

#include <QtWidgets/QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include <QHBoxLayout> 
#include "StatusBar.h"
#include <QtGamepad/qgamepad.h>
#include <qglobal.h>
#include <QtCore/qlogging.h>
#include <QMessageLogger>
//#include <Xinput.h>
#include "ControllerMonitor.h"
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

	//Top GroupBox
	QGroupBox *gamepad;
	QHBoxLayout *gamepadLayout;

	//Left Pane Box
	QGroupBox *buttons;
	QFormLayout *buttonsLayout;

	//Right Pane Box
	QGroupBox *trigger_Analog;
	QFormLayout *trigger_AnalogLayout;

	//Middle Log groupBox
	QGroupBox *logGroupBox;
	QVBoxLayout *logFrameLayout;
	
	
	//Bottom groupBox
	QGroupBox *controls_status;
	QGridLayout *controls_statusLayout;
	QGroupBox *serverMode;
	QVBoxLayout *serverModeLayout;
	QGroupBox *controlServer;
	QHBoxLayout *controlServerLayout;

	//widgets
	QPlainTextEdit *logBox;
	QPushButton *StartServer;
	QPushButton *StopServer;
	QComboBox *serverModeOptions;
	QLabel *ServerConnected;
	enum options {Server, MCU, Debug};
	Q_ENUM(options);

	//Labels and line Edit
	QLabel *labelA;
	QLabel *labelB;
	QLabel *labelX;
	QLabel *labelY;
	QLabel *labelLeftTrigger;
	QLabel *labelRightAnalog;
	QLabel *labelLeftAnalog;
	QLabel *labelRightTrigger;
	QLabel *ipLabel;
	QLabel *portLabel;

	QLineEdit *lineEditA;
	QLineEdit *lineEditB;
	QLineEdit *lineEditX;
	QLineEdit *lineEditY;
	QLineEdit *lineEditLeftTrigger;
	QLineEdit *lineEditRightAnalog;
	QLineEdit *lineEditLeftAnalog;
	QLineEdit *lineEditRightTrigger;
	QLineEdit *ipLineEdit;
	QLineEdit *portLineEdit;
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
	void tryConnect();
	void sendData(QByteArray data);
};
