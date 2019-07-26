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
#include <QTcpSocket>
//#include <Xinput.h>
#include "ControllerMonitor.h"

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
	QGroupBox *logGroupBox;
	QVBoxLayout *logFrameLayout;	
	QPlainTextEdit *logBox;
	QPushButton *testMe;

	//Gamepad Listener
	ControllerMonitor *gamepads;
	bool controllerDefined;
	void startListener(void);
	void closeEvent(QCloseEvent *event);

	//TCP Transmitter and Receiver
	QTcpSocket *Transmitter;
	QTcpSocket *Receiver;

public slots:
	void logSlot(QString message);
	void getControllerState(void);
	void handleNewState(Controller *newState);
signals:
	void logSignal(QString message);
	void monitor(void);
};