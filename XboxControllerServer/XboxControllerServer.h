#pragma once
#pragma comment(lib, "XInput.lib")

#include <QtWidgets/QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include <QHBoxLayout> 
#include "StatusBar.h"
//#include <boost/log/trivial.hpp>
#include <QtGamepad/qgamepad.h>
#include <qglobal.h>
#include <QtCore/qlogging.h>
#include <QMessageLogger>
#include <Xinput.h>


class XboxControllerServer : public QMainWindow
{
	Q_OBJECT

public:
	XboxControllerServer(QWidget *parent = Q_NULLPTR);

	// Fields and Main Window variables
	QWidget *centralWidget;
	QVBoxLayout *centralLayout;
	QIcon *Icon;
	DefaultStatusBar *statusBar;

	//GroupBoxes 
	QGroupBox *logGroupBox;
	QVBoxLayout *logFrameLayout;
	
	QPlainTextEdit *logBox;
	QPushButton *testMe;

	QGamepad *controller;

public slots:
	void logSlot(QString message);
	void getControllerState(void);
signals:
	void logSignal(QString message);
};