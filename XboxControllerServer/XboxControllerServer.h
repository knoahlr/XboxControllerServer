#pragma once

#include <QtWidgets/QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include <QHBoxLayout> 
#include "StatusBar.h"
//#include "ui_XboxControllerServer.h"

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
	QGroupBox *rootFolderGroupBox;
	QGroupBox  *archiveFolderGroupBox;
	QGroupBox *archiveButtonGroupBox;
	QFormLayout *rootFolderGroupBoxLayout;
	QFormLayout *archiveFolderGroupBoxLayout;
	QVBoxLayout *archiveButtonLayout;


	//Push buttons and LineEdits
	QLineEdit *rootFolderLineEdit;
	QLineEdit *archiveFolderLineEdit;
	QPushButton *rootBrowseButton;
	QPushButton *archiveBrowseButton;
	QPushButton *archiveButton;



	void dirCopier(void);


};
