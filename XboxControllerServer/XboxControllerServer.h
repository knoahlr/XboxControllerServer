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
	QThread Listener;

public:
	XboxControllerServer(QWidget *parent = Q_NULLPTR);

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
	QHBoxLayout *controls_statusLayout;

	//widgets
	QPlainTextEdit *logBox;
	QPushButton *testMe;

	//Labels and line Edit
	QLabel *labelA;
	QLabel *labelB;
	QLabel *labelX;
	QLabel *labelY;
	QLabel *labelLeftTrigger;
	QLabel *labelRightAnalog;
	QLabel *labelLeftAnalog;
	QLabel *labelRightTrigger;

	QLineEdit *lineEditA;
	QLineEdit *lineEditB;
	QLineEdit *lineEditX;
	QLineEdit *lineEditY;
	QLineEdit *lineEditLeftTrigger;
	QLineEdit *lineEditRightAnalog;
	QLineEdit *lineEditLeftAnalog;
	QLineEdit *lineEditRightTrigger;

	//Gamepad Listener
	ControllerMonitor *gamepads;
	bool controllerDefined;
	int controllerRetries = 5;
	int playerID = 0;
	
	//MCU
	QString mcuIP = "192.168.91.112";

	void initializeGUI(void);
	void initializeClient(void);
	void startListener(void);
	void closeEvent(QCloseEvent *event);
	
private:
	TcpClient *Client;


public slots:
	void logSlot(QString message);
	void startServer(void);
	void handleNewState(Controller *newState);
	void tcpResponseHandler(QString data);
; signals:
	void logSignal(QString message);
	void monitor(void);
};