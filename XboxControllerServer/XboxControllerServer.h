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

class XboxControllerServer : public QMainWindow
{

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
	QThread *Listener;
	ControllerMonitor *gamepads;
	void startListener(void);

public slots:
	void logSlot(QString message);
	void getControllerState(void);
	void handleNewState(Controller *newState);
signals:
	void logSignal(QString message);
	void monitor(void);
};