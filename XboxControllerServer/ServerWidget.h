#pragma once
#include <QtWidgets>
#include <qglobal.h>
class ServerWidget : public QWidget
{

	Q_OBJECT

public:
	explicit ServerWidget(QWidget *parent = Q_NULLPTR);


#pragma region serverTab
	QVBoxLayout *serverCentralLayout;

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
	enum options { Server, MCU, Debug };
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
#pragma endregion serverTab

	int currentMode(void);
};